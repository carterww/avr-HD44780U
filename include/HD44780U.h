#ifndef HD44780U_H
#define HD44780U_H

#include <stdint.h>
#include <avr/io.h>

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

/* DDRAM address values. Each line has 27 (0x1B) characters.
 * Add the offset to line's home to reach that character's
 * address
 */
#define LCD_DDRAM_ADDRESS_FIRST_LINE_HOME 0x0
#define LCD_DDRAM_ADDRESS_SECOND_LINE_HOME 0x28

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

/* Initializes the LCD by setting certain parameters as outlined below:
 * 1. Function Set: Sets the LCD to 4 or 8 bit mode, number of
 *    display lines, and character font (dot count). For more information on these
 *    parameters, view the function lcd_function_set.
 * 2. Display Control: Sets the display to on/off (always on in this function), cursor
 *    to on/off, and cursor blink to on/off. For more information on these parameters,
 *    view the function lcd_display_control.
 * 3. Entry Mode: Sets the cursor move direction (whether DDRAM increments or decrements)
 *    and display shift. For more information on these parameters, view the function
 *    lcd_entry_mode_set.
 * 4. Clear Display: Clears the display and puts the cursor home (DDRAM address 0x0).
 */
void lcd_init(struct lcd_pins *pins, uint8_t bit_mode, uint8_t line_num, uint8_t dot_count,
        uint8_t show_cursor, uint8_t blink, uint8_t cursor_direction, uint8_t display_shift);

/* Set settings related to string writing behavior.
 * @param new_line_behavior: Either LCD_SETTINGS_IGNORE_NEW_LINE or LCD_SETTINGS_WRAP_NEW_LINE.
 * Whether to wrap to next line (if on line 1 and is a 2 line display) or ignore '\n' in
 * strings.
 * @return: 0 if valid settings, something else if invalid settings.
 */
int8_t lcd_change_settings(uint8_t new_line_behavior);

/* Writes a string to the display at current cursor position.
 * @param str: NULL terminated string to write to the display.
 */
void lcd_write_string(struct lcd_pins *pins, char *str);

/* Sets the cursor to the appropriate position. (0 indexed)
 * @param x_pos: horizontal position.
 * @param y_pos: vertical position.
 */
void lcd_set_cursor(struct lcd_pins *pins, uint8_t x_pos, uint8_t y_pos);

/* Clears the display and sets the cursor to the home position (DDRAM address 0x0). */
void lcd_clear_display(struct lcd_pins *pins);

/* Puts the cursor at the home position (DDRAM address 0x0), but does not clear
 * characters from DDRAM. If the display is shifted, it returns to default
 * position
 */
void lcd_return_home(struct lcd_pins *pins);

/* Sets the cursors move direction (left or right) and display shift.
 * @param cursor_direction: Either LCD_ENTRY_MODE_FLAG_SHIFT_DECREMENT or
 * LCD_ENTRY_MODE_FLAG_SHIFT_INCREMENT. Decrement causes the cursor (or display)
 * to move to the left because the DDRAM address is decremented on each read/write.
 * Increment causes the cursor (or display) to move to the right.
 * @param display_shift: Either LCD_ENTRY_MODE_FLAG_NO_SHIFT or LCD_ENTRY_MODE_FLAG_SHIFT.
 * Whether the display will shift or not on each read/write.
 */
void lcd_entry_mode_set(struct lcd_pins *pins, uint8_t cursor_direction, uint8_t display_shift);


/* Sets properties relating to the display.
 * @param display: Either LCD_DISPLAY_CONTROL_FLAG_DISPLAY_OFF or
 * LCD_DISPLAY_CONTROL_FLAG_DISPLAY_ON. Simply turns the display on or off.
 * @param show_cursor: Either LCD_DISPLAY_CONTROL_FLAG_CURSOR_OFF or
 * LCD_DISPLAY_CONTROL_FLAG_CURSOR_ON. Whether to highlight the cursor's
 * position.
 * @param blink: Either LCD_DISPLAY_CONTROL_FLAG_BLINK_OFF or 
 * LCD_DISPLAY_CONTROL_FLAG_BLINK_ON. Blinks at the cursor's position if turned
 * on.
 */
void lcd_display_control(struct lcd_pins *pins, uint8_t display, uint8_t show_cursor,
        uint8_t blink);

/* Moves the cursor or display without changing DDRAM contents.
 * @param display_or_cursor: Either LCD_DORC_SHIFT_FLAG_DISPLAY or
 * LCD_DORC_SHIFT_FLAG_CURSOR. Whether to shift the whole display or just
 * the cursor.
 * @params direction: Either LCD_DORC_SHIFT_FLAG_RIGHT or LCD_DORC_SHIFT_FLAG_LEFT.
 * Direction to shift the display or cursor.
 */
void lcd_dorc_shift(struct lcd_pins *pins, uint8_t display_or_cursor, uint8_t direction);

/* Used to set parameters for the LCD that should not change after first call.
 * @param bit_mode: Either LCD_FUNCTION_SET_FLAG_8BIT_MODE or LCD_FUNCTION_SET_FLAG_4BIT_MODE.
 * Mode corresponding to whether all (D0-D7) or half (D4-D7) of the data lines are being used.
 * @param line_num: Either LCD_FUNCTION_SET_FLAG_2LINE or LCD_FUNCTION_SET_FLAG_1LINE.
 * Number of lines on display.
 * @param dot_count: Either LCD_FUNCTION_SET_FLAG_5x10_DOTS or LCD_FUNCTION_SET_FLAG_5x8_DOTS.
 * Specifies the font to be used. There are a wider range of 5x8 dot characters.
 */
void lcd_function_set(struct lcd_pins *pins, uint8_t bit_mode, uint8_t line_num,
        uint8_t dot_count);

/* Resets the LCD via the reset command sequence outlined on page 45 or 46
 * of the datasheet. Bit mode is required here because setting the lcd
 * to 4 bit mode calls for an extra command. Called in lcd_init.
 * @param pins: See the struct lcd_pins
 * @param bit_mode: Either LCD_8BIT_MODE or LCD_4BIT_MODE
 */
void lcd_reset(struct lcd_pins *pins, uint8_t bit_mode);

/* Polls the busy flag and waits while the LCD is busy. */
void lcd_wait_busy(struct lcd_pins *pins);

/* Function used to write a command/data in 4 bit mode */
void lcd_write_4bit(struct lcd_pins *pins, uint8_t byte, uint8_t rs);

/* Function used to write a command/data in 8 bit mode */
void lcd_write_8bit(struct lcd_pins *pins, uint8_t byte, uint8_t rs);

/* Function used to write a command/data in 4 bit mode */
uint8_t lcd_read_4bit(struct lcd_pins *pins, uint8_t rs);

/* Function used to write a command/data in 8 bit mode */
uint8_t lcd_read_8bit(struct lcd_pins *pins, uint8_t rs);

/* Reads the busy flag of the LCD (D7).
 * NOTE: Address counter register value is in DB0-DB6, so make sure to
 * only check DB7 if bitwisign for BF.
 */
uint8_t lcd_read_busy_flag(struct lcd_pins *pins);

#endif
