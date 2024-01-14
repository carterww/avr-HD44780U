#include "HD44780U/commands.h"
#include "HD44780U/defs.h"

#include "../internal/defs.h"
#include "../internal/lcd_pulse_enable.h"

#include "avr/io.h"
#include "util/delay.h"

extern struct _lcd_settings lcd_settings;

void lcd_reset(struct lcd_pins *pins, uint8_t bit_mode)
{
    _delay_ms(25);
    for (uint8_t i = 0; i < 3; i++) {
        SET_PIN(pins->rs_port, pins->rs_pin, LCD_REGISTER_COMMAND);
        SET_PIN(pins->rw_port, pins->rw_pin, LCD_WRITE_MODE);
        uint8_t i = GET_DATA_START(); 
        for (; i < 8; i++) {
            if (_BV(i) & (LCD_FUNC_FUNCTION_SET | LCD_FUNCTION_SET_FLAG_8BIT_MODE))
                SET_PIN(pins->data_port[i], pins->data_pin[i], 1);
            else
                SET_PIN(pins->data_port[i], pins->data_pin[i], 0);
        }
        pulse_enable(pins);
        _delay_ms(6);
    }
    if (GET_DATA_START() == 4) {
        SET_PIN(pins->rs_port, pins->rs_pin, LCD_REGISTER_COMMAND);
        SET_PIN(pins->rw_port, pins->rw_pin, LCD_WRITE_MODE);
        uint8_t i = GET_DATA_START(); 
        for (; i < 8; i++) {
            if (_BV(i) & (LCD_FUNC_FUNCTION_SET | LCD_FUNCTION_SET_FLAG_4BIT_MODE))
                SET_PIN(pins->data_port[i], pins->data_pin[i], 1);
            else
                SET_PIN(pins->data_port[i], pins->data_pin[i], 0);
        }
        pulse_enable(pins);
        _delay_ms(6);
    }
}
