#include "HD44780U/commands.h"
#include "HD44780U/defs.h"

#include "../internal/defs.h"

#include "avr/io.h"

extern struct _lcd_settings lcd_settings;

int8_t lcd_change_settings(uint8_t new_line_behavior)
{
    if (new_line_behavior == LCD_SETTINGS_WRAP_NEW_LINE)
        lcd_settings.settings |= _BV(_NEW_LINE_BIT);
    else if (new_line_behavior == LCD_SETTINGS_IGNORE_NEW_LINE)
        lcd_settings.settings &= ~(_BV(_NEW_LINE_BIT));
    else
        return 1;
    return 0;
}

