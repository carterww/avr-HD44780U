#include "HD44780U.h"
#include <avr/io.h>
#include <util/delay.h>

/* Data structure to hold the ports and pins */
static struct lcd_pins pins;

int main(void)
{
    /* Set the pins and ports (They can all be on different ports) */
    pins.rs_port = &PORTD;
    pins.rw_port = &PORTD;
    pins.en_port = &PORTD;
    pins.rs_pin = PD5;
    pins.rw_pin = PD6;
    pins.en_pin = PD7;
    for (int i = 0; i < 8; i++) {
        if (i < 4) {
            pins.data_port[i] = 0;
            pins.data_pin[i] = 0;
        } else {
            pins.data_port[i] = &PORTB;
            pins.data_pin[i] = PB0 + (i - 4);
        }
    }

    /* Init the LCD with all the params */
    lcd_init(&pins, LCD_FUNCTION_SET_FLAG_4BIT_MODE, LCD_FUNCTION_SET_FLAG_2LINE,
            LCD_FUNCTION_SET_FLAG_5x8_DOTS, LCD_DISPLAY_CONTROL_FLAG_CURSOR_OFF,
            LCD_DISPLAY_CONTROL_FLAG_BLINK_OFF, LCD_ENTRY_MODE_FLAG_SHIFT_INCREMENT,
            LCD_ENTRY_MODE_FLAG_NO_SHIFT);

    /* Make it wrap to a new line when it encounter '\n' */
    lcd_change_settings(LCD_SETTINGS_WRAP_NEW_LINE);

    /* Write "Hello World!" and some other stuff across screen */
    lcd_write_string(&pins, "Hello\nWorld!");
    lcd_set_cursor(&pins, 8, 0);
    lcd_write_string(&pins, "Here\'s");
    lcd_set_cursor(&pins, 8, 1);
    lcd_write_string(&pins, "Something!");
    lcd_set_cursor(&pins, 21, 0);
    lcd_write_string(&pins, "Over");
    lcd_set_cursor(&pins, 21, 1);
    lcd_write_string(&pins, "Here.");
    lcd_set_cursor(&pins, 28, 0);
    lcd_write_string(&pins, "Alright");
    lcd_set_cursor(&pins, 28, 1);
    lcd_write_string(&pins, "That's it.");

    /* Shift display to the left every 300 ms to show all the words */
    while (1) {
        lcd_dorc_shift(&pins, LCD_DORC_SHIFT_FLAG_DISPLAY, LCD_DORC_SHIFT_FLAG_LEFT);
        _delay_ms(300);
    }
}
