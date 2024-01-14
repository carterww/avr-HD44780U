#include "HD44780U/write.h"
#include "HD44780U/defs.h"

#include "../internal/lcd_pulse_enable.h"

#include "avr/io.h"
#include "util/delay.h"

void lcd_write_8bit(struct lcd_pins *pins, uint8_t byte, uint8_t rs)
{
    SET_PIN(pins->rw_port, pins->rw_pin, LCD_WRITE_MODE);
    SET_PIN(pins->rs_port, pins->rs_pin, rs);

    for (uint8_t i = 0; i < 8; i++) {
        uint8_t val = byte & (_BV(i));
        SET_PIN(pins->data_port[i], pins->data_pin[i], val);
    }
    pulse_enable(pins); // Internal, need to define
    _delay_us(5);
}
