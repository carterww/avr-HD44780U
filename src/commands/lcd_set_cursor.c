#include "HD44780U/commands.h"
#include "HD44780U/write.h"
#include "HD44780U/defs.h"

#include "../internal/write_helpers.h"
#include "../internal/defs.h"

#include "avr/io.h"

extern struct _lcd_settings lcd_settings;

void lcd_set_cursor(struct lcd_pins *pins, uint8_t x_pos, uint8_t y_pos)
{
    uint8_t ddram_address = y_pos * LCD_DDRAM_ADDRESS_SECOND_LINE_HOME;
    ddram_address += x_pos;
    lcd_wait_busy(pins);
    LCD_WRITE(pins, LCD_FUNC_SET_DDRAM_ADDRESS | ddram_address, LCD_REGISTER_COMMAND);
}
