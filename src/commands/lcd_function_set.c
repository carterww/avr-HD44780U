#include "HD44780U/commands.h"
#include "HD44780U/write.h"
#include "HD44780U/defs.h"

#include "../internal/write_helpers.h"
#include "../internal/defs.h"

#include "avr/io.h"

extern struct _lcd_settings lcd_settings;

void lcd_function_set(struct lcd_pins *pins, uint8_t bit_mode, uint8_t line_num,
        uint8_t dot_count)
{
    lcd_wait_busy(pins);
    LCD_WRITE(pins, LCD_FUNC_FUNCTION_SET | bit_mode | line_num | dot_count,
            LCD_REGISTER_COMMAND);
}
