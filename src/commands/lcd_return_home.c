#include "HD44780U/commands.h"
#include "HD44780U/write.h"
#include "HD44780U/defs.h"

#include "../internal/write_helpers.h"
#include "../internal/defs.h"

#include "avr/io.h"
#include "util/delay.h"

extern struct _lcd_settings lcd_settings;

void lcd_return_home(struct lcd_pins *pins)
{
    lcd_wait_busy(pins);
    LCD_WRITE(pins, LCD_FUNC_RETURN_HOME, LCD_REGISTER_COMMAND);
    _delay_ms(7); // These take extra time. LCD was ignoring some instructions right after
}
