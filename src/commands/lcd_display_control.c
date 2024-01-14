#include "HD44780U/commands.h"
#include "HD44780U/write.h"
#include "HD44780U/defs.h"

#include "../internal/write_helpers.h"
#include "../internal/defs.h"

#include "avr/io.h"

extern struct _lcd_settings lcd_settings;

void lcd_display_control(struct lcd_pins *pins, uint8_t display, uint8_t show_cursor,
        uint8_t blink)
{
    lcd_wait_busy(pins);
    // Internal, need to define
    LCD_WRITE(pins, LCD_FUNC_DISPLAY_CONTROL | display | show_cursor | blink,
            LCD_REGISTER_COMMAND);
}
