BUILD_DIR = build
SRC_DIR = src
INC_DIR = include
LIB_DIR = lib

LIB_NAME = HD44780U
LIB_FILE_NAME = lib$(LIB_NAME).a

# MCU stuff
BAUD = 9600
FREQ = 16000000UL
MMCU = atmega328p

# AVR dude stuff
PORT = /dev/ttyACM0
PROGRAMMER = arduino
# MCU to partno found here https://www.nongnu.org/avrdude/user-manual/avrdude_3.html#Option-Descriptions
PARTNO = m328p
AVRDUDEBAUD = 115200

SRC_FILES = $(wildcard $(SRC_DIR)/*/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))


default: library

library: $(BUILD_DIR) $(LIB_DIR)
	$(foreach file,$(SRC_FILES),$(shell avr-gcc -DBAUD=$(BAUD) -DF_CPU=$(FREQ) -O2 -mmcu=$(MMCU) -I$(INC_DIR) -c $(file) -o $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(file)) -ffunction-sections -fdata-sections))
	avr-ar rcs $(LIB_DIR)/$(LIB_FILE_NAME) $(OBJ_FILES)

example: library
	avr-gcc -DBAUD=$(BAUD) -DF_CPU=$(FREQ) -O2 -mmcu=$(MMCU) -I$(INC_DIR) examples/helloworld.c -o $(BUILD_DIR)/example.o -ffunction-sections -fdata-sections -Wl,--gc-sections -Wl,-static -L$(LIB_DIR) -l$(LIB_NAME)
	avr-objcopy -O ihex -R .eeprom $(BUILD_DIR)/example.o $(BUILD_DIR)/example.hex

burn-example: example
	avrdude -F -V -c $(PROGRAMMER) -p $(PARTNO) -P $(PORT) -b $(AVRDUDEBAUD) -U flash:w:$(BUILD_DIR)/example.hex

$(BUILD_DIR):
	mkdir $(BUILD_DIR)
	mkdir $(BUILD_DIR)/commands
	mkdir $(BUILD_DIR)/gpio
	mkdir $(BUILD_DIR)/internal
	mkdir $(BUILD_DIR)/read
	mkdir $(BUILD_DIR)/write

$(LIB_DIR):
	mkdir $(LIB_DIR)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(LIB_DIR)
	rm -f main
	rm -f main.hex
