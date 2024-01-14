#ifndef _HD44780_WRITE_H_
#define _HD44780_WRITE_H_

/* This file contains the functions used to write to the LCD.
 * These operate directly on the GPIO pins and carry out the
 * commands sent by command.h.
 */

#include "HD44780U/defs.h"
#include <stdint.h>

/* Function used to write a command/data in 4 bit mode */
void lcd_write_4bit(struct lcd_pins *pins, uint8_t byte, uint8_t rs);

/* Function used to write a command/data in 8 bit mode */
void lcd_write_8bit(struct lcd_pins *pins, uint8_t byte, uint8_t rs);

#endif // _HD44780_WRITE_H_
