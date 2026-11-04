// MFRC522 SPI driver with register diagnostics and four-byte UID reads.
// Register address: (reg << 1) & 0x7E; set bit 7 for reads.

#include "mfrc522.h"
#include "pico/stdlib.h"
#include <string.h>

#define CommandReg     0x01
#define ComIEnReg      0x02
#define ComIrqReg      0x04
#define ErrorReg       0x06
#define Status2Reg     0x08
#define FIFODataReg    0x09
#define FIFOLevelReg   0x0A
#define ControlReg     0x0C
#define BitFramingReg  0x0D
#define ModeReg        0x11
#define TxModeReg      0x12
#define RxModeReg      0x13
#define TxControlReg   0x14
#define TxASKReg       0x15
#define ModWidthReg    0x24
#define RFCfgReg       0x26
#define GsNReg         0x27
#define TModeReg       0x2A
#define TPrescalerReg  0x2B
#define TReloadRegH    0x2C
#define TReloadRegL    0x2D
#define VersionReg     0x37

#define PCD_IDLE       0x00
#define PCD_TRANSCEIVE 0x0C
#define PCD_RESETPHASE 0x0F

#define PICC_REQIDL    0x26
#define PICC_WUPA      0x52
#define PICC_ANTICOLL  0x93

static spi_inst_t *s_spi;
static uint s_cs, s_rst;

int     mfrc522_fail_stage = 0;
uint8_t mfrc522_atqa[2]    = { 0, 0 };
uint8_t mfrc522_raw[16]    = { 0 };
uint8_t mfrc522_raw_len    = 0;
uint8_t mfrc522_irq        = 0;
uint8_t mfrc522_err        = 0;
uint8_t mfrc522_bitframing = 0;
uint8_t mfrc522_selftest_last = 0;
uint8_t mfrc522_cfg_fail      = 0;
uint8_t mfrc522_cfg_badreg    = 0;
uint8_t mfrc522_cfg_badval    = 0;

static inline void cs_low(void)  { gpio_put(s_cs, 0); sleep_us(1); }
static inline void cs_high(void) { sleep_us(1); gpio_put(s_cs, 1); }

static void write_reg(uint8_t reg, uint8_t val) {
    uint8_t buf[2] = { (uint8_t)((reg << 1) & 0x7E), val };
    cs_low();
    spi_write_blocking(s_spi, buf, 2);
    cs_high();
}

static uint8_t read_reg(uint8_t reg) {
    uint8_t addr = (uint8_t)(((reg << 1) & 0x7E) | 0x80);
    uint8_t val = 0;
    cs_low();
    spi_write_blocking(s_spi, &addr, 1);

    spi_read_blocking(s_spi, 0x00, &val, 1);
    cs_high();
    return val;
}

static void set_bits(uint8_t reg, uint8_t mask) {
    write_reg(reg, read_reg(reg) | mask);
}
static void clear_bits(uint8_t reg, uint8_t mask) {
    write_reg(reg, read_reg(reg) & (uint8_t)~mask);
}

// Retry register writes and record configuration failures.
static void write_verify(uint8_t reg, uint8_t val) {
    for (int i = 0; i < 5; i++) {
        write_reg(reg, val);
        if (read_reg(reg) == val) return;
        sleep_ms(1);
    }
    mfrc522_cfg_fail++;
    mfrc522_cfg_badreg = reg;
    mfrc522_cfg_badval = read_reg(reg);
}

static void antenna_on(void) {
    if (!(read_reg(TxControlReg) & 0x03)) set_bits(TxControlReg, 0x03);
}

static bool transceive(const uint8_t *send, uint8_t send_len,
                       uint8_t *recv, uint8_t *recv_len,
                       uint8_t bitframing) {
    write_reg(CommandReg, PCD_IDLE);
    write_reg(ComIEnReg, 0x77 | 0x80);
    clear_bits(ComIrqReg, 0x80);
    set_bits(FIFOLevelReg, 0x80);

    for (uint8_t i = 0; i < send_len; i++) write_reg(FIFODataReg, send[i]);

    write_reg(BitFramingReg, bitframing);
    write_reg(CommandReg, PCD_TRANSCEIVE);
    write_reg(BitFramingReg, (uint8_t)(bitframing | 0x80));

    mfrc522_bitframing = read_reg(BitFramingReg);

    uint16_t guard = 2000;
    uint8_t irq;
    do {
        irq = read_reg(ComIrqReg);
        guard--;
    } while (guard && !(irq & 0x01) && !(irq & 0x30));

    clear_bits(BitFramingReg, 0x80);

    mfrc522_irq     = irq;
    mfrc522_err     = read_reg(ErrorReg);
    mfrc522_raw_len = 0;

    if (guard == 0) return false;
    if (mfrc522_err & 0x1B) return false;

    if (!(irq & 0x20)) return false;

    uint8_t n = read_reg(FIFOLevelReg);
    if (n > *recv_len) n = *recv_len;
    for (uint8_t i = 0; i < n; i++) recv[i] = read_reg(FIFODataReg);
    *recv_len = n;

    mfrc522_raw_len = n > sizeof(mfrc522_raw) ? sizeof(mfrc522_raw) : n;
    for (uint8_t i = 0; i < mfrc522_raw_len; i++) mfrc522_raw[i] = recv[i];

    return true;
}

void mfrc522_init(spi_inst_t *spi, uint cs_pin, uint rst_pin) {
    s_spi = spi;
    s_cs  = cs_pin;
    s_rst = rst_pin;

    gpio_init(s_cs);  gpio_set_dir(s_cs,  GPIO_OUT); gpio_put(s_cs, 1);
    gpio_init(s_rst); gpio_set_dir(s_rst, GPIO_OUT);

    gpio_put(s_rst, 0); sleep_ms(10);
    gpio_put(s_rst, 1); sleep_ms(50);

    write_reg(CommandReg, PCD_RESETPHASE);
    sleep_ms(50);

    mfrc522_cfg_fail = 0;

    write_verify(TModeReg,      0x8D);
    write_verify(TPrescalerReg, 0x3E);
    write_verify(TReloadRegL,   30);
    write_verify(TReloadRegH,   0);
    write_verify(TxASKReg,      0x40);
    write_verify(ModeReg,       0x3D);

    write_verify(TxModeReg,   0x00);
    write_verify(RxModeReg,   0x00);
    write_verify(ModWidthReg, 0x26);
    write_verify(GsNReg,      0xFF);
    write_verify(RFCfgReg,    0x70);

    antenna_on();
}

uint8_t mfrc522_version(void) {
    return read_reg(VersionReg);
}

uint8_t mfrc522_reg(uint8_t reg) {
    return read_reg(reg);
}

void mfrc522_set_reg(uint8_t reg, uint8_t val) {
    write_reg(reg, val);
}

int mfrc522_selftest(int rounds) {

    static const uint8_t pattern[] = { 0x55, 0xAA, 0x0F, 0xF0, 0x26 };
    int bad = 0;

    for (int r = 0; r < rounds; r++) {
        for (unsigned i = 0; i < sizeof(pattern); i++) {
            write_reg(ModWidthReg, pattern[i]);
            mfrc522_selftest_last = read_reg(ModWidthReg);
            if (mfrc522_selftest_last != pattern[i]) bad++;
        }
    }

    write_reg(ModWidthReg, 0x26);
    return bad;
}

// Try REQA, then WUPA; read anticollision data and verify BCC.
bool mfrc522_read_uid(uint8_t uid[4]) {
    uint8_t buf[16];
    uint8_t len;

    uint8_t req;
    len = sizeof(buf);
    req = PICC_REQIDL;
    if (!transceive(&req, 1, buf, &len, 0x07) || len != 2) {
        len = sizeof(buf);
        req = PICC_WUPA;
        if (!transceive(&req, 1, buf, &len, 0x07)) { mfrc522_fail_stage = 1; return false; }
        if (len != 2)                              { mfrc522_fail_stage = 2; return false; }
    }
    mfrc522_atqa[0] = buf[0];
    mfrc522_atqa[1] = buf[1];

    uint8_t anti[2] = { PICC_ANTICOLL, 0x20 };
    len = sizeof(buf);
    if (!transceive(anti, 2, buf, &len, 0x00)) { mfrc522_fail_stage = 3; return false; }
    if (len != 5)                              { mfrc522_fail_stage = 4; return false; }

    uint8_t bcc = buf[0] ^ buf[1] ^ buf[2] ^ buf[3];
    if (bcc != buf[4])                   { mfrc522_fail_stage = 5; return false; }

    memcpy(uid, buf, 4);
    mfrc522_fail_stage = 0;
    return true;
}
