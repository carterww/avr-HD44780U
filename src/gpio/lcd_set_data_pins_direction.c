#include "HD44780U/gpio.h"
#include "HD44780U/defs.h"

#include "../internal/defs.h"

#include "avr/io.h"

extern struct _lcd_settings lcd_settings;

void lcd_set_data_pins_direction(struct lcd_pins *pins, uint8_t direction)
{
    uint8_t i = GET_DATA_START(); // Internal, need to define
    for (; i < 8; i++) {
        volatile uint8_t *ddrx_port = portx_to_ddrx(pins->data_port[i]);
        SET_PIN(ddrx_port, pins->data_pin[i], direction);
    }
}
