#ifndef _LCD_INTERNAL_PULSE_ENABLE_H
#define _LCD_INTERNAL_PULSE_ENABLE_H

#include "HD44780U/defs.h"
#include "defs.h"

#include <util/delay.h>

void pulse_enable(struct lcd_pins *pins);

#endif // _LCD_INTERNAL_PULSE_ENABLE_H
