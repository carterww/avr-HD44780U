# HD44780U LCD AVR Driver
Library for interfacing AVR MCUs with a HD44780U based LCD display. Just a personal library used for tinkering and learning.

# Interfacing with this Library
The library can be make using GNU make. By default, make outputs a "libHD44780U.a" in the "./lib" directory. This can then be compiled into your program (see make example to see how). Headers are in the "./include/HD44780U" directory. Object and hex files get put into the "./build" directory. See "examples/helloworld.c" for a simple program that interfaces with the library, and burn the example onto your MCU by running "make burn-example." Make sure to adjust the Makefile variables according to your MCU.

# Example Program on Arduino Uno
https://github.com/carterww/avr-HD44780U/assets/82002721/4769500f-aad8-458a-b3bd-7569c9b5d1fb


## Datasheets
- [HD44780U Datasheet](https://cdn-shop.adafruit.com/datasheets/HD44780.pdf)
- [My LCD's Datasheet](https://www.digikey.ca/htmldatasheets/production/2410219/0/0/1/tc1602a-01t.html)
