#ifndef _HD44780U_GPIO_H_
#define _HD44780U_GPIO_H_

/* This file contains all the helper functions related to
 * the GPIO pins used to interface with the HD44780U LCD.
 */

#include <stdint.h>
#include "HD44780U/defs.h"

/* Sets the data pins (D0-D7 or D4-D7) to the given direction.
 * @param pins: The pins struct.
 * @param value: The value to set the pins to (input=0, 1=output).
 */
void lcd_set_data_pins_direction(struct lcd_pins *pins, uint8_t direction);

/* Gets the DDRx register (data direction) from the given
 * PORTx register (data register). Used to set the DDRx
 * register.
 * @param portx: The PORT's address (e.g. &PORTB)
 */
volatile uint8_t *portx_to_ddrx(volatile uint8_t *portx);

#endif // _HD44780U_GPIO_H_
