#include "HD44780U/defs.h"
#include "defs.h"

#include <util/delay.h>

void pulse_enable(struct lcd_pins *pins) {
    TOGGLE_EN(pins, 0);
    TOGGLE_EN(pins, 1);
    TOGGLE_EN(pins, 0);
}
