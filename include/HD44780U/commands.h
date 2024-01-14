#ifndef _HD44780_COMMANDS_H_
#define _HD44780_COMMANDS_H_

/* File that holds all the commands that can be sent to the LCD
 * and some higher level functions that use these commands. The
 * lowest level functions are in write.h and read.h.
 */

#include "HD44780U/defs.h"

/* START HIGHER LEVEL FUNCTIONS */

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

/* Resets the LCD via the reset command sequence outlined on page 45 or 46
 * of the datasheet. Bit mode is required here because setting the lcd
 * to 4 bit mode calls for an extra command. Called in lcd_init.
 * @param pins: See the struct lcd_pins
 * @param bit_mode: Either LCD_8BIT_MODE or LCD_4BIT_MODE
 */
void lcd_reset(struct lcd_pins *pins, uint8_t bit_mode);

/* Polls the busy flag and waits while the LCD is busy.
 * TODO: Might just switch to a delay instead of polling
 * because max execution time is 1.64ms with the average being
 * about 41 us.
 */
void lcd_wait_busy(struct lcd_pins *pins);

/* END HIGHER LEVEL FUNCTIONS */

/* START LOWER LEVEL FUNCTIONS (DIRECT COMMANDS) */

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

/* END LOWER LEVEL FUNCTIONS (DIRECT COMMANDS) */

#endif // _HD44780_COMMANDS_H_
