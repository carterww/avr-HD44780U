#ifndef _LCD_INTERNAL_READ_HELPERS_H_
#define _LCD_INTERNAL_READ_HELPERS_H_

#define LCD_READ(pins, rs) (lcd_settings.settings & (_BV(_BIT_MODE_BIT))) ? \
    (lcd_read_8bit(pins, rs)) : \
    (lcd_read_4bit(pins, rs))

#endif // _LCD_INTERNAL_READ_HELPERS_H_
