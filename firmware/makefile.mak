COM=COM3
BAUDRATE=115200
PROJECT=firmware
DEVICE=ATmega328P

DFP_PATH="C:\Users\gabriel.correa\Documents\Projects\Embedded Systems\Packages\AtmegaPackage\xc8"
CC_PATH="C:\Users\gabriel.correa\Microchip\xc8\v3.10\bin"
CC="$(CC_PATH)\xc8-cc.exe"

#CC_PATH="C:/Users/gabriel.correa/.platformio/packages/toolchain-atmelavr/bin"
#CC="$(CC_PATH)\avr-g++.exe"

BUILD=build/default/production
DIST=dist/default/production
HEX_PATH="./$(DIST)/$(PROJECT).X.production.hex"

OBJCP="$(CC_PATH)\avr-objcopy"
OBJDUMP="$(CC_PATH)\avr-objdump.exe"

#OBJCP = "$(CC_PATH)/avr-objcopy"
#OBJDUMP = "$(CC_PATH)/avr-objdump"
#SIZE = "$(CC_PATH)/avr-size"

SRCS=$(wildcard *.c) $(wildcard drivers/*.c)
#SRCS=$(wildcard *.cpp)
HDRS=$(wildcard *.h) $(wildcard drivers/*.h)

BUILDS=$(SRCS:%.c=$(BUILD)/%.o)
#BUILDS=$(SRCS:%.cpp=$(BUILD)/%.o)

#	$(CC)  	-mcpu=$(DEVICE) \\
#			-mdfp=$(DFP_PATH) \\
#			-DXPRJ_default=default  \
#			-Wl,--defsym=__MPLAB_BUILD=1 \
#			-gdwarf-3 \
#			-Wl,--memorysummary,$(DIST)/memoryfile.xml \
			-mno-const-data-in-progmem      \

all: build $(BUILDS)
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(CC)  	-mcpu=$(DEVICE) \
			-mdfp=$(DFP_PATH) \
	      	-I. \
			-DXPRJ_default=default  \
			-Wl,--defsym=__MPLAB_BUILD=1 \
			-gdwarf-3 \
			-Wl,--memorysummary,$(DIST)/memoryfile.xml \
			-mno-const-data-in-progmem      \
			-Wl,-Map=$(DIST)/$(PROJECT).X.production.map  \
			-Wl,--gc-sections -O1 \
			-ffunction-sections \
			-fdata-sections \
			-fshort-enums \
			-fno-common \
			-funsigned-char \
			-funsigned-bitfields \
			-Wall \
			-o $(DIST)/$(PROJECT).X.production.elf  \
			-o $(DIST)/$(PROJECT).X.production.elf  \
			$(BUILDS) \
			-Wl,--start-group  \
			-Wl,-lm -Wl,--end-group
	$(OBJCP) -O ihex "$(DIST)/$(PROJECT).X.production.elf" \
			"$(DIST)/$(PROJECT).X.production.hex"
	avr-objdump -Pmem-usage "$(DIST)/$(PROJECT).X.production.elf"

$(BUILD)/%.o: %.c
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(CC)  	-mcpu=$(DEVICE) \
			-mdfp=$(DFP_PATH) \
	      	-I. \
			-c -x c -D__$(DEVICE)__ \
			-Wl,--gc-sections -O1 \
			-ffunction-sections \
			-fdata-sections \
			-fshort-enums \
			-fno-common \
			-funsigned-char \
			-funsigned-bitfields \
			-Wall -DXPRJ_default=default \
			-gdwarf-3 \
			-mno-const-data-in-progmem \
			-MD -MP -MF "$@.d" \
			-MT "$@.d" \
			-MT $@ \
			-o $@ \
			$<


build:
	mkdir "$(BUILD)"
	mkdir "$(DIST)"


notepad:
	@C:\Program Files\Notepad++\notepad++.exe main.c $(SRCS) $(HDRS)

flash:
	@C:\Users\gabriel.correa\AVRDUDESS\avrdude.exe -c arduino -p m328p -P $(COM) -b $(BAUDRATE) -U flash:w:$(HEX_PATH):a 
#	avrdude -c arduino -p ATmega328P -p $(COM) -b $(BAUDRATE) -U flash:w:$(HEX_PATH):a 


flasher:
	@C:\Users\gabriel.correa\AVRDUDESS\avrdudess.exe


