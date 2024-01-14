PORT = /dev/ttyACM0
BAUD = 9600
FREQ = 16000000UL

default: example

example:
	avr-gcc -DBAUD=$(BAUD) -DF_CPU=$(FREQ) -O2 -mmcu=atmega328p -I./include src/HD44780U.c examples/helloworld.c -o main
# I need to work on these. Make some for library and examples
#build:

burn: example
	avr-objcopy -O ihex -R .eeprom main main.hex
	avrdude -F -V -c arduino -p m328p -P $(PORT) -b 115200 -U flash:w:main.hex
