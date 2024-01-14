#ifndef _LCD_INTERNAL_DEFS_H_
#define _LCD_INTERNAL_DEFS_H_

#include <stdint.h>
#include "avr/io.h"

/* Bit positions for inernal settings */
#define _LINE_LEN_BIT 0
#define _NEW_LINE_BIT 1
#define _BIT_MODE_BIT 2

struct _lcd_settings {
    uint8_t settings;
};

/* Gets starting index for data_pins and data_ports based on bit mode */
#define GET_DATA_START() (lcd_settings.settings & (_BV(_BIT_MODE_BIT))) ? (0) : (4)

#define TOGGLE_EN(pins, val) { \
     SET_PIN(pins->en_port, pins->en_pin, val); \
     _delay_us(1); \
}

#endif // _LCD_INTERNAL_DEFS_H_
