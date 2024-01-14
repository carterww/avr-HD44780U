#include "HD44780U/gpio.h"
#include "HD44780U/defs.h"

#include "avr/io.h"

volatile uint8_t *portx_to_ddrx(volatile uint8_t *port)
{
    if (&PORTB == port) return &DDRB;
    if (&PORTD == port) return &DDRD;
    // PORTC is analog on chip, never used for this
    return 0;
}
