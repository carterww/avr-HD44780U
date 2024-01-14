#ifndef HD44780U_DEFS_H
#define HD44780U_DEFS_H

/* This file contains all the definitions needed for the
 * HD44780U LCD driver. This includes the various commands
 * that can be sent to the LCD, the various flags that can
 * be set for each command, and the various settings that
 * can be set for the LCD.
 */

#include <stdint.h>

/* Various modes to bit value */
#define LCD_REGISTER_COMMAND 0
#define LCD_REGISTER_DATA 1
#define LCD_WRITE_MODE 0
#define LCD_READ_MODE 1

/* When sending a command/data to the LCD, the EN pin
 * must be pulsed. If we pulse too fast, the command/data
 * may get ignored, so we must delay.
 * NOTE: This value may need to be changed under certain conditions.
 */
#define LCD_ENABLE_PULSE_DELAY_US 1

/* Bits used for specific commands with flags set to 0.
 * These should be OR'd with user provided flags
 * to send the command with proper parameters.
 */
#define LCD_FUNC_CLEAR_DISPLAY 0x01
#define LCD_FUNC_RETURN_HOME 0x02
#define LCD_FUNC_ENTRY_MODE_SET 0x04
#define LCD_FUNC_DISPLAY_CONTROL 0x08
#define LCD_FUNC_DORC_SHIFT 0x10          // DORC = display or cursor
#define LCD_FUNC_FUNCTION_SET 0x20
#define LCD_FUNC_SET_CGRAM_ADDRESS 0x40
#define LCD_FUNC_SET_DDRAM_ADDRESS 0x80

/* lcd_entry_mode_set flags to combine with
 * LCD_FUNC_ENTRY_MODE_SET
 */
#define LCD_ENTRY_MODE_FLAG_NO_SHIFT 0x00
#define LCD_ENTRY_MODE_FLAG_SHIFT 0x01
#define LCD_ENTRY_MODE_FLAG_SHIFT_DECREMENT 0x00
#define LCD_ENTRY_MODE_FLAG_SHIFT_INCREMENT 0x02

/* lcd_display_control flags to combine with
 * LCD_FUNC_DISPLAY_CONTROL
 */
#define LCD_DISPLAY_CONTROL_FLAG_DISPLAY_OFF 0x00
#define LCD_DISPLAY_CONTROL_FLAG_DISPLAY_ON 0x04
#define LCD_DISPLAY_CONTROL_FLAG_CURSOR_OFF 0x00
#define LCD_DISPLAY_CONTROL_FLAG_CURSOR_ON 0x02
#define LCD_DISPLAY_CONTROL_FLAG_BLINK_OFF 0x00
#define LCD_DISPLAY_CONTROL_FLAG_BLINK_ON 0x01

/* lcd_cursor_shift flags to combine with
 * LCD_FUNC_DORC_SHIFT
 */
#define LCD_DORC_SHIFT_FLAG_DISPLAY 0x08
#define LCD_DORC_SHIFT_FLAG_CURSOR 0x00
#define LCD_DORC_SHIFT_FLAG_RIGHT 0x04
#define LCD_DORC_SHIFT_FLAG_LEFT 0x00

/* lcd_function_set flags to combine with
 * LCD_FUNC_FUNCTION_SET
 */
#define LCD_FUNCTION_SET_FLAG_8BIT_MODE 0x10
#define LCD_FUNCTION_SET_FLAG_4BIT_MODE 0x00
#define LCD_FUNCTION_SET_FLAG_2LINE 0x08
#define LCD_FUNCTION_SET_FLAG_1LINE 0x00
#define LCD_FUNCTION_SET_FLAG_5x10_DOTS 0x04
#define LCD_FUNCTION_SET_FLAG_5x8_DOTS 0x00

/* DDRAM address values. Each line has 40 characters.
 * Add the offset to line's home to reach that character's
 * address
 */
#define LCD_DDRAM_ADDRESS_FIRST_LINE_HOME 0x0
#define LCD_DDRAM_ADDRESS_SECOND_LINE_HOME 0x40

/* Settings parameters for writing strings */
#define LCD_SETTINGS_IGNORE_NEW_LINE 0x0
#define LCD_SETTINGS_WRAP_NEW_LINE 0x2

/* Struct to store information needed for translating GPIO
 * pins to the LCD's data and control pin. This struct will
 * be passed into each function.
 */
struct lcd_pins {
    volatile uint8_t* data_port[8];
    volatile uint8_t* rs_port;
    volatile uint8_t* rw_port;
    volatile uint8_t* en_port;

    uint8_t data_pin[8];
    uint8_t rs_pin;
    uint8_t rw_pin;
    uint8_t en_pin;
};

/* Macro for setting pins easily (assumes that the pins are output)
 * @param port: address to the port the pin is on.
 * @param pin: pin number on the port.
 * @param val: value to set the pin to (0 or 1).
 */
#define SET_PIN(port, pin, val) (val) ? (*port |= _BV(pin)) : (*port &= ~(_BV(pin)))

#endif // HD44780U_DEFS_H
