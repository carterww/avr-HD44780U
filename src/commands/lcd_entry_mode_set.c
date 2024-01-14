#include "HD44780U/commands.h"
#include "HD44780U/write.h"
#include "HD44780U/defs.h"

#include "../internal/write_helpers.h"
#include "../internal/defs.h"

#include "avr/io.h"

extern struct _lcd_settings lcd_settings;

void lcd_entry_mode_set(struct lcd_pins *pins, uint8_t cursor_direction, uint8_t display_shift)
{
    lcd_wait_busy(pins);
    LCD_WRITE(pins, LCD_FUNC_ENTRY_MODE_SET | cursor_direction |
            display_shift, LCD_REGISTER_COMMAND);
}
