// MFRC522 SPI driver and diagnostics; four-byte UID support.

#ifndef MFRC522_H
#define MFRC522_H

#include <stdbool.h>
#include <stdint.h>
#include "hardware/spi.h"

void mfrc522_init(spi_inst_t *spi, uint cs_pin, uint rst_pin);

uint8_t mfrc522_version(void);

// Fill a four-byte UID on success.
bool mfrc522_read_uid(uint8_t uid[4]);

// Failure stage: 0 success, 1 no response, 2 bad ATQA length,
// 3 anticollision failed, 4 bad UID length, 5 BCC mismatch.
extern int mfrc522_fail_stage;

// Last ATQA response; invalid if the request failed.
extern uint8_t mfrc522_atqa[2];

// Last received bytes and length.
extern uint8_t mfrc522_raw[16];
extern uint8_t mfrc522_raw_len;

// Last ComIrqReg and ErrorReg values.
extern uint8_t mfrc522_irq;
extern uint8_t mfrc522_err;

extern uint8_t mfrc522_bitframing;

// Return register write/read mismatch count; restore ModWidthReg afterward.
int mfrc522_selftest(int rounds);
extern uint8_t mfrc522_selftest_last;

// Failed initialization writes, last failing register, and observed value.
extern uint8_t mfrc522_cfg_fail;
extern uint8_t mfrc522_cfg_badreg;
extern uint8_t mfrc522_cfg_badval;

void mfrc522_set_reg(uint8_t reg, uint8_t val);

uint8_t mfrc522_reg(uint8_t reg);

#endif
