#include "HD44780U/commands.h"
#include "HD44780U/write.h"
#include "HD44780U/defs.h"

#include "../internal/write_helpers.h"
#include "../internal/defs.h"

#include "avr/io.h"

extern struct _lcd_settings lcd_settings;

void lcd_write_string(struct lcd_pins *pins, char *str)
{
    while (*str) {
        if (*str == '\n' && (lcd_settings.settings & (_BV(_NEW_LINE_BIT)))) {
            lcd_set_cursor(pins, 0, 1);
            goto inc;
        }
        lcd_wait_busy(pins);
        LCD_WRITE(pins, *(str), LCD_REGISTER_DATA);
inc:
        str++;
    }
}
