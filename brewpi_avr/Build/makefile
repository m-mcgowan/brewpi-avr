
#
# Copyright 2013 BrewPi/Elco Jacobs.
# Copyright 2014 Matthew McGowan
#
# This file is part of BrewPi.
# 
# BrewPi is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# BrewPi is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
#

# convert build parameters to defines

# simulator flag control by shield type - the simulator is a virtual shield
BREWPI_SIMULATOR = 0

ifeq ($(SHIELD),diy)
	BREWPI_STATIC_CONFIG = BREWPI_SHIELD_DIY
else ifeq ($(SHIELD),revA)
	BREWPI_STATIC_CONFIG = BREWPI_SHIELD_REV_A
else ifeq ($(SHIELD),revC)
	BREWPI_STATIC_CONFIG = BREWPI_SHIELD_REV_C
else ifeq ($(SHIELD),simulator)
	BREWPI_SIMULATOR = 1
endif

## build target directory organization and output files

# base name of output files
TARGET_NAME ?= brewpi-avr

# location of .cpp sources
SRC=../

# temporary .o and .d files
OBJ_DIR = tmp/

# location of artefacts to keep
OUTPUT_DIR ?= final/

# default locations for tools based on the toolchain bin dir
AVRGCC ?= $(TOOLCHAIN_BIN)avr-g++$(EXE)
AVROBJCOPY ?= $(TOOLCHAIN_BIN)avr-objcopy$(EXE)
AVROBJDUMP ?= $(TOOLCHAIN_BIN)avr-objdump$(EXE)
AVRSIZE ?= $(TOOLCHAIN_BIN)avr-size$(EXE)

ARDUINO_VERSION ?= 105

# default config if nothing else specified
BOARD ?= leonardo
BREWPI_STATIC_CONFIG ?= BREWPI_SHIELD_REV_C
BREWPI_DEBUG ?= 0
BUILD_NUMBER ?= 0
BUILD_NAME ?= 00000000

DEFINES = \
 -DBREWPI_STATIC_CONFIG=$(BREWPI_STATIC_CONFIG) \
 -DBREWPI_DEBUG=$(BREWPI_DEBUG) \
 -DBREWPI_SIMULTE=$(BREWPI_SIMULATOR) \
 -DARDUINO=$(ARDUINO_VERSION) \
 -DF_CPU=16000000L \
 -DBUILD_NUMBER=$(BUILD_NUMBER) \
 -DBUILD_NAME="\"$(BUILD_NAME)"\"
 
ifeq ($(BOARD),leonardo)
	MCU = atmega32u4
	MCU_HEADERS = leonardo
else ifeq ($(BOARD),uno)
	MCU = atmega328p
	MCU_HEADERS = standard
endif

ARDUINO_HEADERS ?= $(SRC)arduino/cores/arduino

MCU_INCLUDE_VARIANT ?= $(SRC)arduino/variants
MCU_INCLUDE ?= $(MCU_INCLUDE_VARIANT)/$(MCU_HEADERS)

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS :=  \
$(SRC)fallback


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
$(SRC)ArduinoFunctions.cpp \
$(SRC)Brewpi.cpp \
$(SRC)Commands.cpp \
$(SRC)Comms.cpp \
$(SRC)Main.cpp \
$(SRC)Ticks.cpp \
$(SRC)Values.cpp

BASENAMES += $(notdir $(basename $(C_SRCS)))
 
PREPROCESSING_SRCS += 

ASM_SRCS += 

OBJS += $(addprefix $(OBJ_DIR),$(addsuffix .o,$(BASENAMES)))
OBJS_AS_ARGS = $(OBJS)
C_DEPS += $(addprefix $(OBJ_DIR),$(addsuffix .d, $(BASENAMES)))
C_DEPS_AS_ARGS = $(C_DEPS)


OUTPUT_FILE_PATH +=$(OUTPUT_DIR)$(TARGET_NAME).elf

OUTPUT_FILE_PATH_AS_ARGS +=$(OUTPUT_DIR)$(TARGET_NAME).elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

# AVR32/GNU C Compiler


#
# Recipes
#

./$(OBJ_DIR)%.o: $(SRC)./%.cpp
	@echo Building file: $<
	@echo Invoking: AVR8/GNU C Compiler
	"$(AVRGCC)" $(DEFINES) -mcall-prologues -funsigned-char -funsigned-bitfields -I"$(ARDUINO_HEADERS)" -I"$(MCU_INCLUDE)" -I"$(SRC)/fallback"  -Os -fno-split-wide-types -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g3 -Wall -mmcu=$(MCU) -c -fno-exceptions -ffreestanding -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$(OBJ_DIR)" "$<"
	@echo Finished building: $<
	
# AVR32/GNU Preprocessing Assembler

# AVR32/GNU Assembler

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP)
	@echo Building target: $@
	@echo Invoking: AVR8/GNU Linker
	"$(AVRGCC)" -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="$(OUTPUT_DIR)$(TARGET_NAME).map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=$(MCU) -Wl,--relax 
	@echo Finished building target: $@
	"$(AVROBJCOPY)" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "$(OUTPUT_DIR)$(TARGET_NAME).elf" "$(OUTPUT_DIR)$(TARGET_NAME).hex"
	
	"$(AVROBJCOPY)" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "$(OUTPUT_DIR)$(TARGET_NAME).elf" "$(OUTPUT_DIR)$(TARGET_NAME).eep" || exit 0
	
	"$(AVROBJDUMP)" -h -S "$(OUTPUT_DIR)$(TARGET_NAME).elf" > "$(OUTPUT_DIR)$(TARGET_NAME).lss"
	
	"$(AVROBJCOPY)" -O srec -R .eeprom -R .fuse -R .lock -R .signature  "$(OUTPUT_DIR)$(TARGET_NAME).elf" "$(OUTPUT_DIR)$(TARGET_NAME).srec"
	
	"$(AVRSIZE)" "$(OUTPUT_DIR)$(TARGET_NAME).elf" > "$(OUTPUT_DIR)$(TARGET_NAME).elf.size"



# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	-$(RM) "$(OUTPUT_DIR)$(TARGET_NAME).elf" "$(OUTPUT_DIR)$(TARGET_NAME).a" "$(OUTPUT_DIR)$(TARGET_NAME).hex" "$(OUTPUT_DIR)$(TARGET_NAME).lss" "$(OUTPUT_DIR)$(TARGET_NAME).eep" "$(OUTPUT_DIR)$(TARGET_NAME).map" "$(OUTPUT_DIR)$(TARGET_NAME).srec"
