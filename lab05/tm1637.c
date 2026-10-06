// Bit-banged TM1637 driver for a four-digit display.

#include "tm1637.h"

#define TM_CMD_DATA   0x40
#define TM_CMD_ADDR   0xC0
#define TM_CMD_DISP   0x88
#define TM_DELAY_US   10

static uint tm_clk = 4;
static uint tm_dio = 5;
static uint8_t tm_bright = 7;

static const uint8_t DIGIT[10] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
};
#define SEG_BLANK 0x00
#define SEG_DASH  0x40

// Drive low or release to the pull-up; never drive high.
static void pin_low(uint pin)     { gpio_set_dir(pin, GPIO_OUT); gpio_put(pin, 0); }
static void pin_release(uint pin) { gpio_set_dir(pin, GPIO_IN); }
static void pin_set(uint pin, bool high) { high ? pin_release(pin) : pin_low(pin); }

static void tm_start(void) {
    pin_low(tm_dio);   sleep_us(TM_DELAY_US);
    pin_low(tm_clk);   sleep_us(TM_DELAY_US);
}

static void tm_stop(void) {
    pin_low(tm_dio);      sleep_us(TM_DELAY_US);
    pin_release(tm_clk);  sleep_us(TM_DELAY_US);
    pin_release(tm_dio);  sleep_us(TM_DELAY_US);
}

// Send LSB first, then release DIO for the ACK cycle.
static void tm_write_byte(uint8_t b) {
    for (int i = 0; i < 8; i++) {
        pin_set(tm_dio, (b >> i) & 1);  sleep_us(TM_DELAY_US);
        pin_release(tm_clk);            sleep_us(TM_DELAY_US);
        pin_low(tm_clk);                sleep_us(TM_DELAY_US);
    }
    pin_release(tm_dio);   sleep_us(TM_DELAY_US);
    pin_release(tm_clk);   sleep_us(TM_DELAY_US);
    pin_low(tm_clk);       sleep_us(TM_DELAY_US);
}

static void tm_cmd(uint8_t c) {
    tm_start();
    tm_write_byte(c);
    tm_stop();
}

void tm1637_init(uint clk_pin, uint dio_pin) {
    tm_clk = clk_pin;
    tm_dio = dio_pin;

    gpio_init(tm_clk);  gpio_pull_up(tm_clk);  pin_release(tm_clk);
    gpio_init(tm_dio);  gpio_pull_up(tm_dio);  pin_release(tm_dio);
    sleep_us(TM_DELAY_US);

    tm_cmd(TM_CMD_DATA);
    tm_cmd(TM_CMD_DISP | tm_bright);
    tm1637_clear();
}

void tm1637_brightness(uint level) {
    tm_bright = (level > 7) ? 7 : (uint8_t)level;
    tm_cmd(TM_CMD_DATA);
    tm_cmd(TM_CMD_DISP | tm_bright);
}

void tm1637_raw(const uint8_t seg[4]) {
    tm_cmd(TM_CMD_DATA);

    tm_start();
    tm_write_byte(TM_CMD_ADDR);
    for (int i = 0; i < 4; i++) tm_write_byte(seg[i]);
    tm_stop();

    tm_cmd(TM_CMD_DISP | tm_bright);
}

void tm1637_clear(void) {
    const uint8_t blank[4] = { SEG_BLANK, SEG_BLANK, SEG_BLANK, SEG_BLANK };
    tm1637_raw(blank);
}

void tm1637_number(int num) {
    if (num > 9999) num = 9999;
    if (num < -999) num = -999;

    bool neg = (num < 0);
    int v = neg ? -num : num;

    uint8_t seg[4] = { SEG_BLANK, SEG_BLANK, SEG_BLANK, SEG_BLANK };
    int i = 3;
    do {
        seg[i--] = DIGIT[v % 10];
        v /= 10;
    } while (v > 0 && i >= 0);

    if (neg && i >= 0) seg[i] = SEG_DASH;
    tm1637_raw(seg);
}

void tm1637_clock(uint left, uint right, bool colon) {
    if (left  > 99) left  = 99;
    if (right > 99) right = 99;

    uint8_t seg[4] = {
        DIGIT[(left  / 10) % 10], DIGIT[left  % 10],
        DIGIT[(right / 10) % 10], DIGIT[right % 10],
    };
    if (colon) seg[1] |= 0x80;

    tm1637_raw(seg);
}
