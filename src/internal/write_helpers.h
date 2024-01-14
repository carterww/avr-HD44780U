#ifndef _LCD_INTERNAL_WRITE_HELPERS_H_
#define _LCD_INTERNAL_WRITE_HELPERS_H_

#define LCD_WRITE(pins, byte, rs) (lcd_settings.settings & (_BV(_BIT_MODE_BIT))) ? \
    (lcd_write_8bit(pins, byte, rs)) : \
    (lcd_write_4bit(pins, byte, rs))

#endif // _LCD_INTERNAL_WRITE_HELPERS_H_
