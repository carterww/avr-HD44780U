#include "HD44780U/commands.h"

#ifdef LCD_BUSY_WAIT_CHECK
#include "HD44780U/defs.h"
#include "HD44780U/read.h"
#include "HD44780U/gpio.h"

#include "../internal/defs.h"
#include "../internal/read_helpers.h"

#include "avr/io.h"

extern struct _lcd_settings lcd_settings;

#else
#include "util/delay.h"
#endif


void lcd_wait_busy(struct lcd_pins *pins)
{
#ifdef LCD_BUSY_WAIT_CHECK
    /* I was having issues when I didn't clear these */
    for (uint8_t i = GET_DATA_START(); i < 8; i++) {
        SET_PIN(pins->data_port[i], pins->data_pin[i], 0);
    }

    lcd_set_data_pins_direction(pins, 0);
    while ((LCD_READ(pins, LCD_REGISTER_COMMAND)) & _BV(7))
        ;

    lcd_set_data_pins_direction(pins, 1);
#else
    _delay_us(41);
#endif
}
