#include "HD44780U/commands.h"
#include "HD44780U/write.h"
#include "HD44780U/defs.h"

#include "../internal/write_helpers.h"
#include "../internal/defs.h"

#include "avr/io.h"

extern struct _lcd_settings lcd_settings;

void lcd_dorc_shift(struct lcd_pins *pins, uint8_t display_or_cursor, uint8_t direction)
{
    lcd_wait_busy(pins);
    LCD_WRITE(pins, LCD_FUNC_DORC_SHIFT | display_or_cursor | direction,
            LCD_REGISTER_COMMAND);
}
