#include "HD44780U/defs.h"
#include "HD44780U/commands.h"

#include <avr/io.h>
#include <util/delay.h>

/* Data structure to hold the ports and pins */
static struct lcd_pins pins;

int main(void)
{
    /* Set the pins and ports (They can all be on different ports).
     * If using 4 bit mode, leave first 4 ports and pins as 0.
     */
    pins = (struct lcd_pins) {
        .rs_port = &PORTD,
        .rw_port = &PORTD,
        .en_port = &PORTD,
        .rs_pin = PD5,
        .rw_pin = PD6,
        .en_pin = PD7,
        .data_port = { 0, 0, 0, 0, &PORTB, &PORTB, &PORTB, &PORTB },
        .data_pin = { 0, 0, 0, 0, PB0, PB1, PB2, PB3 }
    };

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
