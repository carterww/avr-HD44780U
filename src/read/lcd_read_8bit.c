#include "HD44780U/read.h"
#include "HD44780U/defs.h"

#include "../internal/defs.h"

#include "avr/io.h"
#include "util/delay.h"

uint8_t lcd_read_8bit(struct lcd_pins *pins, uint8_t rs)
{
    uint8_t res = 0;

    SET_PIN(pins->rw_port, pins->rw_pin, LCD_READ_MODE);
    SET_PIN(pins->rs_port, pins->rs_pin, rs);
    _delay_us(1);
    TOGGLE_EN(pins, 1); // Internal, need to define

    for (uint8_t i = 0; i < 8; i++) {
        if (_BV(pins->data_pin[i]) & *pins->data_port[i])
            res |= _BV(i);
    }

    TOGGLE_EN(pins, 0);
    return res;
}
