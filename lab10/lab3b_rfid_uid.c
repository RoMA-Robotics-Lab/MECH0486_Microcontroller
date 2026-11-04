// Read four-byte card UIDs and compare them with the registered list.
// MISO GP4, CS GP5, SCK GP6, MOSI GP7, RST GP22; use 3V3 only.

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "mfrc522.h"

#define SPI_PORT  spi0
#define SPI_HZ    (1000 * 1000)
#define PIN_MISO  4
#define PIN_CS    5
#define PIN_SCK   6
#define PIN_MOSI  7
#define PIN_RST   22

#define POLL_MS     50
#define GONE_COUNT   4
#define SHOW_DIAG    1

#define TxControlReg 0x14
#define ModeReg        0x11
#define RxThresholdReg 0x18
#define CWGsPReg       0x28
#define ModGsPReg      0x29
#define TxModeReg    0x12
#define RxModeReg    0x13
#define TxASKReg     0x15
#define ModWidthReg  0x24
#define RFCfgReg     0x26
#define GsNReg       0x27

static int try_reads(int n) {
    uint8_t uid[4];
    int ok = 0;
    for (int i = 0; i < n; i++) {
        if (mfrc522_read_uid(uid)) ok++;
        sleep_ms(10);
    }
    return ok;
}

// Keep a card on the antenna while testing read success rates.
static void autotune(void) {
    static const uint8_t rxth[] = { 0x84, 0x64, 0x44, 0x34 };
    static const uint8_t gsp[]  = { 0x20, 0x30, 0x3F };

    int     best = -1;
    uint8_t best_rx = 0x84, best_gs = 0x20;

    printf("자동 튜닝 - 카드를 코일에 붙인 채 기다린다 (약 10초)\n");

    for (unsigned i = 0; i < sizeof(rxth); i++) {
        for (unsigned j = 0; j < sizeof(gsp); j++) {
            mfrc522_set_reg(RxThresholdReg, rxth[i]);
            mfrc522_set_reg(CWGsPReg,  gsp[j]);
            mfrc522_set_reg(ModGsPReg, gsp[j]);

            int ok = try_reads(20);
            printf("  RxTh=0x%02X GsP=0x%02X : 20번 중 %2d번\n", rxth[i], gsp[j], ok);

            if (ok > best) { best = ok; best_rx = rxth[i]; best_gs = gsp[j]; }
        }
    }

    mfrc522_set_reg(RxThresholdReg, best_rx);
    mfrc522_set_reg(CWGsPReg,  best_gs);
    mfrc522_set_reg(ModGsPReg, best_gs);
    printf("  -> RxTh=0x%02X GsP=0x%02X 선택 (최고 %d/20)\n\n", best_rx, best_gs, best);
}

static const char *version_text(uint8_t v) {
    if (v == 0x91 || v == 0x92) return "(MFRC522 정품)";
    if (v == 0x88 || v == 0xB2) return "(호환 칩 - 레지스터 규약은 같다)";
    if (v == 0x00 || v == 0xFF) return "(응답 없음 - 배선 확인)";
    return "(알 수 없는 값)";
}

static const char *stage_text(int s) {
    switch (s) {
    case 1:  return "카드 응답 없음 (REQA)";
    case 2:  return "ATQA 길이 이상";
    case 3:  return "UID 응답 없음 (ANTICOLL)";
    case 4:  return "UID 길이 이상 - 7바이트 UID 카드?";
    case 5:  return "BCC 불일치";
    default: return "성공";
    }
}

static const struct {
    uint8_t     uid[4];
    const char *name;
} registered[] = {
    { { 0xDE, 0xAD, 0xBE, 0xEF }, "카드 1" },
    { { 0x12, 0x34, 0x56, 0x78 }, "카드 2" },
};

static int find_registered(const uint8_t uid[4]) {
    for (int i = 0; i < (int)(sizeof(registered) / sizeof(registered[0])); i++)
        if (memcmp(uid, registered[i].uid, 4) == 0) return i;
    return -1;
}

int main(void) {
    stdio_init_all();
    sleep_ms(2000);

    spi_init(SPI_PORT, SPI_HZ);
    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);

    mfrc522_init(SPI_PORT, PIN_CS, PIN_RST);

    printf("\n=== RFID 카드 읽기 ===\n");

    static const uint32_t speeds[] = { 1000000, 500000, 200000, 100000, 50000 };
    uint32_t chosen = 0;

    printf("레지스터 자가진단 (속도별, 각 500번)\n");
    for (unsigned i = 0; i < sizeof(speeds) / sizeof(speeds[0]); i++) {
        spi_set_baudrate(SPI_PORT, speeds[i]);
        int bad = mfrc522_selftest(100);
        printf("  %7lu Hz : %3d번 틀림  %s\n",
               (unsigned long)speeds[i], bad, bad == 0 ? "OK" : "");
        if (bad == 0 && chosen == 0) chosen = speeds[i];
    }

    if (chosen == 0) {
        printf("  -> 모든 속도에서 틀린다. 배선·납땜 문제다.\n");
        chosen = SPI_HZ;
    } else {
        printf("  -> %lu Hz로 진행한다.\n", (unsigned long)chosen);
    }
    spi_set_baudrate(SPI_PORT, chosen);

    mfrc522_init(SPI_PORT, PIN_CS, PIN_RST);

    uint8_t ver = mfrc522_version();
    uint8_t tx  = mfrc522_reg(TxControlReg);
    printf("MFRC522 버전: 0x%02X %s\n", ver, version_text(ver));
    printf("안테나: TxControl=0x%02X %s   RFCfg=0x%02X  GsN=0x%02X  ModWidth=0x%02X\n",
           tx, (tx & 0x03) ? "ON" : "OFF",
           mfrc522_reg(RFCfgReg), mfrc522_reg(GsNReg), mfrc522_reg(ModWidthReg));
    if (mfrc522_cfg_fail)
        printf("★ 설정 %u개가 안 들어감. 마지막: 0x%02X 레지스터에 0x%02X가 읽힘\n",
               mfrc522_cfg_fail, mfrc522_cfg_badreg, mfrc522_cfg_badval);
    else
        printf("설정 쓰기: 전부 확인됨\n");
    printf("송신설정: TxASK=0x%02X  Mode=0x%02X  TxMode=0x%02X  RxMode=0x%02X\n",
           mfrc522_reg(TxASKReg), mfrc522_reg(ModeReg),
           mfrc522_reg(TxModeReg), mfrc522_reg(RxModeReg));
    printf("\n");

    autotune();

    printf("카드를 대면 UID가 찍힌다.\n\n");

    uint8_t  uid[4], last[4] = { 0 };
    bool     present = false;
    unsigned miss    = 0;
    unsigned n_detect = 0, n_known = 0, n_unknown = 0;
    uint64_t diag_at = time_us_64() + 2000000;
    unsigned n_try = 0, n_ok = 0;

    while (true) {
        n_try++;
        if (mfrc522_read_uid(uid)) {
            n_ok++;
            miss = 0;

            if (!present || memcmp(uid, last, 4) != 0) {
                present = true;
                memcpy(last, uid, 4);
                n_detect++;

                printf("[%u] 카드 감지  UID: %02X %02X %02X %02X  -> ",
                       n_detect, uid[0], uid[1], uid[2], uid[3]);

                int idx = find_registered(uid);
                if (idx >= 0) { n_known++;   printf("등록 카드 (%s)\n", registered[idx].name); }
                else          { n_unknown++; printf("미등록 카드\n"); }
            }
        } else if (present && ++miss >= GONE_COUNT) {
            present = false;
            miss    = 0;
            printf("    카드 치움   (감지 %u / 등록 %u / 미등록 %u)\n",
                   n_detect, n_known, n_unknown);
        }

#if SHOW_DIAG

        if (time_us_64() >= diag_at) {
            diag_at += 2000000;
            printf("    [%u/%u 성공, %u%%]", n_ok, n_try,
                   n_try ? (n_ok * 100 / n_try) : 0);
            if (n_ok == 0)
                printf("  단계 %d - %s   ComIrq=0x%02X Error=0x%02X",
                       mfrc522_fail_stage, stage_text(mfrc522_fail_stage),
                       mfrc522_irq, mfrc522_err);
            printf("\n");
            n_try = 0;
            n_ok  = 0;
        }
#endif

        sleep_ms(POLL_MS);
    }
}
