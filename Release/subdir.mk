################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../A02_PWM.cpp \
../A02_UART.cpp \
../A02_UART_Auto.cpp \
../A02_UART_Controlled.cpp \
../DistanceSensor.cpp \
../Template.cpp \
../sloeber.ino.cpp 

LINK_OBJ += \
./A02_PWM.cpp.o \
./A02_UART.cpp.o \
./A02_UART_Auto.cpp.o \
./A02_UART_Controlled.cpp.o \
./DistanceSensor.cpp.o \
./Template.cpp.o \
./sloeber.ino.cpp.o 

CPP_DEPS += \
./A02_PWM.cpp.d \
./A02_UART.cpp.d \
./A02_UART_Auto.cpp.d \
./A02_UART_Controlled.cpp.d \
./DistanceSensor.cpp.d \
./Template.cpp.d \
./sloeber.ino.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
A02_PWM.cpp.o: ../A02_PWM.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/Users/corhofman/Eclipse/embedcpp-2024-062/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/bin/avr-g++" -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10812 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR   -I"/Users/corhofman/Eclipse/embedcpp-2024-062/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.6/cores/arduino" -I"/Users/corhofman/Eclipse/embedcpp-2024-062/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.6/variants/mega" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 "$<" -o "$@"
	@echo 'Finished building: $<'
	@echo ' '

A02_UART.cpp.o: ../A02_UART.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/Users/corhofman/Eclipse/embedcpp-2024-062/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/bin/avr-g++" -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10812 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR   -I"/Users/corhofman/Eclipse/embedcpp-2024-062/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.6/cores/arduino" -I"/Users/corhofman/Eclipse/embedcpp-2024-062/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.6/variants/mega" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 "$<" -o "$@"
	@echo 'Finished building: $<'
	@echo ' '

A02_UART_Auto.cpp.o: ../A02_UART_Auto.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/Users/corhofman/Eclipse/embedcpp-2024-062/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/bin/avr-g++" -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10812 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR   -I"/Users/corhofman/Eclipse/embedcpp-2024-062/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.6/cores/arduino" -I"/Users/corhofman/Eclipse/embedcpp-2024-062/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.6/variants/mega" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 "$<" -o "$@"
	@echo 'Finished building: $<'
	@echo ' '

A02_UART_Controlled.cpp.o: ../A02_UART_Controlled.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/Users/corhofman/Eclipse/embedcpp-2024-062/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/bin/avr-g++" -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10812 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR   -I"/Users/corhofman/Eclipse/embedcpp-2024-062/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.6/cores/arduino" -I"/Users/corhofman/Eclipse/embedcpp-2024-062/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.6/variants/mega" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 "$<" -o "$@"
	@echo 'Finished building: $<'
	@echo ' '

DistanceSensor.cpp.o: ../DistanceSensor.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/Users/corhofman/Eclipse/embedcpp-2024-062/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/bin/avr-g++" -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10812 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR   -I"/Users/corhofman/Eclipse/embedcpp-2024-062/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.6/cores/arduino" -I"/Users/corhofman/Eclipse/embedcpp-2024-062/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.6/variants/mega" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 "$<" -o "$@"
	@echo 'Finished building: $<'
	@echo ' '

Template.cpp.o: ../Template.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/Users/corhofman/Eclipse/embedcpp-2024-062/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/bin/avr-g++" -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10812 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR   -I"/Users/corhofman/Eclipse/embedcpp-2024-062/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.6/cores/arduino" -I"/Users/corhofman/Eclipse/embedcpp-2024-062/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.6/variants/mega" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 "$<" -o "$@"
	@echo 'Finished building: $<'
	@echo ' '

sloeber.ino.cpp.o: ../sloeber.ino.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/Users/corhofman/Eclipse/embedcpp-2024-062/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/bin/avr-g++" -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10812 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR   -I"/Users/corhofman/Eclipse/embedcpp-2024-062/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.6/cores/arduino" -I"/Users/corhofman/Eclipse/embedcpp-2024-062/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.6/variants/mega" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 "$<" -o "$@"

	@echo 'Finished building: $<'
	@echo ' '


