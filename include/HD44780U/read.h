#ifndef _HD44780_READ_H_
#define _HD44780_READ_H_

/* This file contains the functions used to read from the LCD.
 * These operate directly on the GPIO pins. Make sure to
 * change the pins to input mode before calling these functions,
 * and change them back to output mode after calling these functions.
 *
 * This is done because 99% of the time, the LCD will be in
 * write mode (most users will probably never read except for
 * checking the busy flag).
 */

#include "HD44780U/defs.h"
#include <stdint.h>

/* Function used to write a command/data in 4 bit mode */
uint8_t lcd_read_4bit(struct lcd_pins *pins, uint8_t rs);

/* Function used to write a command/data in 8 bit mode */
uint8_t lcd_read_8bit(struct lcd_pins *pins, uint8_t rs);

#endif // _HD44780_READ_H_
