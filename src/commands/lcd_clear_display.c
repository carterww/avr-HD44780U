#include "HD44780U/commands.h"
#include "HD44780U/write.h"
#include "HD44780U/defs.h"

#include "../internal/write_helpers.h"
#include "../internal/defs.h"

#include "avr/io.h"
#include "util/delay.h"

extern struct _lcd_settings lcd_settings;

void lcd_clear_display(struct lcd_pins *pins)
{
    lcd_wait_busy(pins);
    LCD_WRITE(pins, LCD_FUNC_CLEAR_DISPLAY, LCD_REGISTER_COMMAND); // Internal, need to define
    // lcd was ignoring instructions right after clear display, even after waiting for busy flag
    _delay_ms(7);
}
