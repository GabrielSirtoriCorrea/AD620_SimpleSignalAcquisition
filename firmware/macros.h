// ------------- CPU FREQUENCY -------------
#define F_CPU   16000000UL
// --------------- LIBRARIES ---------------

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

// ---------------- MACROS -----------------
#define write_bit(reg, bit, value) (value ? (reg |= (1<<bit)): (reg &= ~(1<<bit)))
#define read_bit(reg, bit) (reg & (1<<bit))
#define set_duty_cycle(reg, duty) (reg = (uint8_t) (((uint8_t)duty/100)*256)-1)
#define toggle_bit(reg, bit) (reg ^= (1<<bit))
#define setInterruption(reg, bit) (bit ? (reg = 0x01) : (reg = 0x00))

// ----------- HARDWARE MAPPING -----------

#define ADC0_BIT         PC0 // 23