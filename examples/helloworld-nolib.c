#include <avr/io.h>
#include <util/delay.h>

#define DATA_PORT PORTB
#define CONTROL_PORT PORTD

#define RS PD5
#define RW PD6
#define E PD7

#define RS_COMMAND 0
#define RS_DATA 1

#define RW_WRITE 0
#define RW_READ 1

#define E_LOW 0
#define E_HIGH 1
#define E_PULSE_DELAY_US 1

// DB4-DB7 are connected to PB0-PB3, using 4-bit mode
#define DATA_START PB0

void pulse_enable(uint8_t delay_us)
{
    CONTROL_PORT &= ~(1 << E);
    _delay_us(E_PULSE_DELAY_US);
    CONTROL_PORT |= (1 << E);
    _delay_us(E_PULSE_DELAY_US);
    CONTROL_PORT &= ~(1 << E);
    _delay_us(E_PULSE_DELAY_US);
}

void write_4bit_mode(uint8_t byte, uint8_t rs_val)
{
    if (!rs_val) {
        CONTROL_PORT &= ~((1 << RS) | (1 << RW));
    } else {
        CONTROL_PORT |= (1 << RS);
        CONTROL_PORT &= ~(1 << RW);
    }
    _delay_ms(50);

    uint8_t mask = 0;
    uint8_t byte_upper = (byte >> 4);
    uint8_t byte_lower = (byte & 0b00001111);
    for (uint8_t i = 0; i < 4; i++) {
        if (byte_upper & (1 << i)) {
            mask |= (1 << (DATA_START + i));
        }
    }
    DATA_PORT |= mask;
    pulse_enable(E_PULSE_DELAY_US);
    _delay_ms(5); // Should check busy flag but lazy
    DATA_PORT &= ~mask;
    mask = 0;
    for (uint8_t i = 0; i < 4; i++) {
        if (byte_lower & (1 << i)) {
            mask |= (1 << (DATA_START + i));
        }
    }
    DATA_PORT |= mask;
    pulse_enable(E_PULSE_DELAY_US);
    DATA_PORT &= ~mask;
    _delay_ms(5);
}

void lcd_reset(void)
{
    _delay_ms(100); // needs >15ms, but 100ms is safe
    for (uint8_t i = 0; i < 3; i++) {
        CONTROL_PORT &= ~((1 << RS) | (1 << RW));
        DATA_PORT = 0x30;
        pulse_enable(E_PULSE_DELAY_US);
        _delay_ms(10); // needs >4.1ms, but 10ms is safe
    }
    // Write function set without DB4
    CONTROL_PORT &= ~((1 << RS) | (1 << RW));
    DATA_PORT = 0x20;
    pulse_enable(E_PULSE_DELAY_US);
    _delay_ms(10); // needs >4.1ms, but 10ms is safe
}

void lcd_init(void)
{
    write_4bit_mode((1 << 5) | (1 << 3), RS_COMMAND);
    write_4bit_mode(0xF, RS_COMMAND);
    write_4bit_mode(0x1, RS_COMMAND);
    write_4bit_mode((1 << 2) | (1 << 1), RS_COMMAND);
}

int main(void)
{
    DDRB |= _BV(PB3) | _BV(PB2) | _BV(PB1) | _BV(PB0);
    DDRD |= _BV(PD7) | _BV(PD6) | _BV(PD5);
    lcd_reset();
    lcd_init();
    _delay_ms(10);
    char *first_ln = "Hello";
    char *second_ln = "World!";

    while (1) {
        for (int i = 0; i < 9; i++) {
            write_4bit_mode(first_ln[i], RS_DATA);
            _delay_ms(1000);
        }
        write_4bit_mode((1 << 7) | 41, RS_COMMAND);
        for (int i = 0; i < 7; i++) {
            write_4bit_mode(second_ln[i], RS_DATA);
            _delay_ms(1000);
        }
        _delay_ms(10000);
        write_4bit_mode(0x1, RS_COMMAND);
    }
}
