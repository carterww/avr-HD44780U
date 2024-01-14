#include "HD44780U.h"

#include <util/delay.h>
#include <avr/io.h>
#include <stddef.h>

/* Bit position for internal settings */
#define LINE_LEN_BIT 0
#define NEW_LINE_BIT 1
#define BIT_MODE_BIT 2

#define SECOND_LINE_ADDR 0x40;

/* Internal struct for storing settings */
struct lcd_settings {
    /* bit 0: line len 8: 0, line len 16: 1
     * bit 1: ignore \n: 0, wrap on \n: 1
     * bit 2: bit_mode 4=0, 8=1
     */
    uint8_t settings;
} lcd_settings = { 0 };

/* Simple macro that lets setting a pin on a port be a little easier to read */
#define SET_PIN(port, pin, val) (val) ? (*port |= _BV(pin)) : (*port &= ~(_BV(pin)))

/* Gets starting index for data_pins and data_ports based on bit mode */
#define GET_DATA_START() (lcd_settings.settings & (_BV(BIT_MODE_BIT))) ? (0) : (4)

#define LCD_READ(pins, rs) (lcd_settings.settings & (_BV(BIT_MODE_BIT))) ? \
    (lcd_read_8bit(pins, rs)) : \
    (lcd_read_4bit(pins, rs))

#define LCD_WRITE(pins, byte, rs) (lcd_settings.settings & (_BV(BIT_MODE_BIT))) ? \
    (lcd_write_8bit(pins, byte, rs)) : \
    (lcd_write_4bit(pins, byte, rs))

#define TOGGLE_EN(pins, val) { \
     SET_PIN(pins->en_port, pins->en_pin, val); \
     _delay_us(1); \
}

/* Pulses the enable pin to let LCD know instruction is ready
 * on RS, RW, and data lines
 */
static void pulse_enable(struct lcd_pins *pins);

/* Get DDRx port from PORT to set direction */
static volatile uint8_t *portx_to_ddrx(volatile uint8_t *port);

/* Sets the gpio pins to out */
static void init_gpio_pins(struct lcd_pins *pins);

/* Sets the gpio data line pins to input (0) or ouput (1) */
static void set_data_pins_direction(struct lcd_pins *pins, uint8_t direction);

static void pulse_enable(struct lcd_pins *pins)
{
    SET_PIN(pins->en_port, pins->en_pin, 0);
    _delay_us(LCD_ENABLE_PULSE_DELAY_US);
    SET_PIN(pins->en_port, pins->en_pin, 1);
    _delay_us(LCD_ENABLE_PULSE_DELAY_US);
    SET_PIN(pins->en_port, pins->en_pin, 0);
    _delay_us(LCD_ENABLE_PULSE_DELAY_US);
}

static volatile uint8_t *portx_to_ddrx(volatile uint8_t *port)
{
    if (&PORTB == port) return &DDRB;
    if (&PORTD == port) return &DDRD;
    // PORTC is analog on chip
    return 0;
}

static void init_gpio_pins(struct lcd_pins *pins)
{
    /* Set control pins individually */
    volatile uint8_t *en_ddrx_port = portx_to_ddrx(pins->en_port);
    volatile uint8_t *rs_ddrx_port = portx_to_ddrx(pins->rs_port);
    volatile uint8_t *rw_ddrx_port = portx_to_ddrx(pins->rw_port);
    SET_PIN(en_ddrx_port, pins->en_pin, 1);
    SET_PIN(rs_ddrx_port, pins->rs_pin, 1);
    SET_PIN(rw_ddrx_port, pins->rw_pin, 1);

    /* Set data pins */
    set_data_pins_direction(pins, 1);
}

static void set_data_pins_direction(struct lcd_pins *pins, uint8_t direction)
{
    uint8_t i = GET_DATA_START();
    for (; i < 8; i++) {
        volatile uint8_t *ddrx_port = portx_to_ddrx(pins->data_port[i]);
        SET_PIN(ddrx_port, pins->data_pin[i], direction);
    }
}

void lcd_wait_busy(struct lcd_pins *pins)
{
    /* I was having issues when I didn't clear these */
    for (uint8_t i = GET_DATA_START(); i < 8; i++) {
        SET_PIN(pins->data_port[i], pins->data_pin[i], 0);
    }

    set_data_pins_direction(pins, 0);
    while ((LCD_READ(pins, LCD_REGISTER_COMMAND)) & _BV(7))
        ;

    set_data_pins_direction(pins, 1);
}

void lcd_write_4bit(struct lcd_pins *pins, uint8_t byte, uint8_t rs)
{
    SET_PIN(pins->rw_port, pins->rw_pin, LCD_WRITE_MODE);
    SET_PIN(pins->rs_port, pins->rs_pin, rs);

    for (uint8_t i = 4; i < 8; i++) {
        if (byte & _BV(i))
            SET_PIN(pins->data_port[i], pins->data_pin[i], 1);
        else
            SET_PIN(pins->data_port[i], pins->data_pin[i], 0);
    }
    pulse_enable(pins);
    _delay_us(5);
    for (uint8_t i = 4; i < 8; i++) {
        if (byte & _BV((i - 4)))
            SET_PIN(pins->data_port[i], pins->data_pin[i], 1);
        else
            SET_PIN(pins->data_port[i], pins->data_pin[i], 0);
    }
    pulse_enable(pins);
    _delay_us(5);
}

void lcd_write_8bit(struct lcd_pins *pins, uint8_t byte, uint8_t rs)
{
    SET_PIN(pins->rw_port, pins->rw_pin, LCD_WRITE_MODE);
    SET_PIN(pins->rs_port, pins->rs_pin, rs);

    for (uint8_t i = 0; i < 8; i++) {
        uint8_t val = byte & (_BV(i));
        SET_PIN(pins->data_port[i], pins->data_pin[i], val);
    }
    pulse_enable(pins);
}

uint8_t lcd_read_4bit(struct lcd_pins *pins, uint8_t rs)
{
    uint8_t res = 0;
    SET_PIN(pins->rw_port, pins->rw_pin, LCD_READ_MODE);
    SET_PIN(pins->rs_port, pins->rs_pin, rs);
    _delay_us(1);
    TOGGLE_EN(pins, 1);

    for (uint8_t i = 4; i < 8; i++) {
        if (_BV(pins->data_pin[i]) & (*pins->data_port[i]))
            res |= _BV(i);
    }

    TOGGLE_EN(pins, 0)
    TOGGLE_EN(pins, 1)

    for (uint8_t i = 4; i < 8; i++) {
        if (_BV(pins->data_pin[i]) & (*pins->data_port[i]))
            res |= _BV((i - 4));
    }
    TOGGLE_EN(pins, 0)
    return res;
}

uint8_t lcd_read_8bit(struct lcd_pins *pins, uint8_t rs)
{
    uint8_t res = 0;
    set_data_pins_direction(pins, 0);

    SET_PIN(pins->rw_port, pins->rw_pin, LCD_READ_MODE);
    SET_PIN(pins->rs_port, pins->rs_pin, rs);
    pulse_enable(pins);
    if (rs)
        _delay_us(40);  // Read data from ram takes time
    for (uint8_t i = 0; i < 8; i++) {
        if (_BV(pins->data_pin[i]) & *pins->data_port[i])
            res |= _BV(i);
    }
    set_data_pins_direction(pins, 1);
    return res;
}

void lcd_reset(struct lcd_pins *pins, uint8_t bit_mode)
{
    _delay_ms(25);
    for (uint8_t i = 0; i < 3; i++) {
        SET_PIN(pins->rs_port, pins->rs_pin, LCD_REGISTER_COMMAND);
        SET_PIN(pins->rw_port, pins->rw_pin, LCD_WRITE_MODE);
        uint8_t i = GET_DATA_START(); 
        for (; i < 8; i++) {
            if (_BV(i) & (LCD_FUNC_FUNCTION_SET | LCD_FUNCTION_SET_FLAG_8BIT_MODE))
                SET_PIN(pins->data_port[i], pins->data_pin[i], 1);
            else
                SET_PIN(pins->data_port[i], pins->data_pin[i], 0);
        }
        pulse_enable(pins);
        _delay_ms(6);
    }
    if (GET_DATA_START() == 4) {
        SET_PIN(pins->rs_port, pins->rs_pin, LCD_REGISTER_COMMAND);
        SET_PIN(pins->rw_port, pins->rw_pin, LCD_WRITE_MODE);
        uint8_t i = GET_DATA_START(); 
        for (; i < 8; i++) {
            if (_BV(i) & (LCD_FUNC_FUNCTION_SET | LCD_FUNCTION_SET_FLAG_4BIT_MODE))
                SET_PIN(pins->data_port[i], pins->data_pin[i], 1);
            else
                SET_PIN(pins->data_port[i], pins->data_pin[i], 0);
        }
        pulse_enable(pins);
        _delay_ms(6);
    }
}

void lcd_init(struct lcd_pins *pins, uint8_t bit_mode, uint8_t line_num, uint8_t dot_count,
        uint8_t show_cursor, uint8_t blink, uint8_t cursor_direction, uint8_t display_shift)
{
    if (bit_mode == LCD_FUNCTION_SET_FLAG_8BIT_MODE)
        lcd_settings.settings |= _BV(BIT_MODE_BIT);
    else
        lcd_settings.settings &= ~(_BV(BIT_MODE_BIT));

    if (line_num == LCD_FUNCTION_SET_FLAG_1LINE)
        lcd_settings.settings |= _BV(LINE_LEN_BIT);
    else
        lcd_settings.settings &= ~(_BV(LINE_LEN_BIT));

    init_gpio_pins(pins);
    lcd_reset(pins, bit_mode);
    lcd_wait_busy(pins);

    lcd_function_set(pins, bit_mode, line_num, dot_count);
    lcd_display_control(pins, LCD_DISPLAY_CONTROL_FLAG_DISPLAY_ON, show_cursor, blink);
    lcd_entry_mode_set(pins, cursor_direction, display_shift);
    lcd_clear_display(pins);
}

int8_t lcd_change_settings(uint8_t new_line_behavior)
{
    if (new_line_behavior == LCD_SETTINGS_WRAP_NEW_LINE)
        lcd_settings.settings |= _BV(NEW_LINE_BIT);
    else if (new_line_behavior == LCD_SETTINGS_IGNORE_NEW_LINE)
        lcd_settings.settings &= ~(_BV(NEW_LINE_BIT));
    else
        return 1;
    return 0;
}

void lcd_set_cursor(struct lcd_pins *pins, uint8_t x_pos, uint8_t y_pos)
{
    uint8_t ddram_address = y_pos * SECOND_LINE_ADDR;
    ddram_address += x_pos;
    lcd_wait_busy(pins);
    LCD_WRITE(pins, LCD_FUNC_SET_DDRAM_ADDRESS | ddram_address, LCD_REGISTER_COMMAND);
}

void lcd_write_string(struct lcd_pins *pins, char *str)
{
    while (*str) {
        if (*str == '\n') {
            lcd_set_cursor(pins, 0, 1);
            goto inc;
        }
        lcd_wait_busy(pins);
        LCD_WRITE(pins, *(str), LCD_REGISTER_DATA);
inc:
        str++;
    }
}

void lcd_clear_display(struct lcd_pins *pins)
{
    lcd_wait_busy(pins);
    LCD_WRITE(pins, LCD_FUNC_CLEAR_DISPLAY, LCD_REGISTER_COMMAND);
    _delay_ms(7); // These take extra time. LCD was ignoring some instructions right after
}

void lcd_return_home(struct lcd_pins *pins)
{
    lcd_wait_busy(pins);
    LCD_WRITE(pins, LCD_FUNC_RETURN_HOME, LCD_REGISTER_COMMAND);
    _delay_ms(7); // These take extra time. LCD was ignoring some instructions right after
}

void lcd_entry_mode_set(struct lcd_pins *pins, uint8_t cursor_direction, uint8_t display_shift)
{
    lcd_wait_busy(pins);
    LCD_WRITE(pins, LCD_FUNC_ENTRY_MODE_SET | cursor_direction |
            display_shift, LCD_REGISTER_COMMAND);
}

void lcd_display_control(struct lcd_pins *pins, uint8_t display, uint8_t show_cursor,
        uint8_t blink)
{
    lcd_wait_busy(pins);
    LCD_WRITE(pins, LCD_FUNC_DISPLAY_CONTROL | display | show_cursor | blink,
            LCD_REGISTER_COMMAND);
}

void lcd_dorc_shift(struct lcd_pins *pins, uint8_t display_or_cursor, uint8_t direction)
{
    lcd_wait_busy(pins);
    LCD_WRITE(pins, LCD_FUNC_DORC_SHIFT | display_or_cursor | direction,
            LCD_REGISTER_COMMAND);
}

void lcd_function_set(struct lcd_pins *pins, uint8_t bit_mode, uint8_t line_num,
        uint8_t dot_count)
{
    lcd_wait_busy(pins);
    LCD_WRITE(pins, LCD_FUNC_FUNCTION_SET | bit_mode | line_num | dot_count,
            LCD_REGISTER_COMMAND);
}
