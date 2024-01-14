#include "HD44780U/commands.h"
#include "HD44780U/defs.h"
#include "HD44780U/gpio.h"

#include "../internal/defs.h"

#include "avr/io.h"

extern struct _lcd_settings lcd_settings;

static void init_gpio_pins(struct lcd_pins *pins)
{
    /* Set control pins individually */
    volatile uint8_t *en_ddrx_port = portx_to_ddrx(pins->en_port);
    volatile uint8_t *rs_ddrx_port = portx_to_ddrx(pins->rs_port);
    volatile uint8_t *rw_ddrx_port = portx_to_ddrx(pins->rw_port);
    SET_PIN(en_ddrx_port, pins->en_pin, 1);
    SET_PIN(rs_ddrx_port, pins->rs_pin, 1);
    SET_PIN(rw_ddrx_port, pins->rw_pin, 1);

    /* Set data pins */
    lcd_set_data_pins_direction(pins, 1);
}


void lcd_init(struct lcd_pins *pins, uint8_t bit_mode, uint8_t line_num, uint8_t dot_count,
        uint8_t show_cursor, uint8_t blink, uint8_t cursor_direction, uint8_t display_shift)
{
    if (bit_mode == LCD_FUNCTION_SET_FLAG_8BIT_MODE)
        lcd_settings.settings |= _BV(_BIT_MODE_BIT);
    else
        lcd_settings.settings &= ~(_BV(_BIT_MODE_BIT));

    if (line_num == LCD_FUNCTION_SET_FLAG_1LINE)
        lcd_settings.settings |= _BV(_LINE_LEN_BIT);
    else
        lcd_settings.settings &= ~(_BV(_LINE_LEN_BIT));

    init_gpio_pins(pins);
    lcd_reset(pins, bit_mode);
    lcd_wait_busy(pins);

    lcd_function_set(pins, bit_mode, line_num, dot_count);
    lcd_display_control(pins, LCD_DISPLAY_CONTROL_FLAG_DISPLAY_ON, show_cursor, blink);
    lcd_entry_mode_set(pins, cursor_direction, display_shift);
    lcd_clear_display(pins);
}
