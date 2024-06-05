################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
SYSCFG_SRCS += \
../adc_to_uart.syscfg 

C_SRCS += \
../adc_to_uart.c \
./ti_msp_dl_config.c \
C:/ti/mspm0_sdk_2_00_01_00/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0g350x_ticlang.c \
../converter.c \
../dc_1v2.c \
../dl_uart_custom.c \
../flag.c \
../message_adc.c \
../message_log.c 

GEN_CMDS += \
./device_linker.cmd 

GEN_FILES += \
./device_linker.cmd \
./device.opt \
./ti_msp_dl_config.c 

C_DEPS += \
./adc_to_uart.d \
./ti_msp_dl_config.d \
./startup_mspm0g350x_ticlang.d \
./converter.d \
./dc_1v2.d \
./dl_uart_custom.d \
./flag.d \
./message_adc.d \
./message_log.d 

GEN_OPTS += \
./device.opt 

OBJS += \
./adc_to_uart.o \
./ti_msp_dl_config.o \
./startup_mspm0g350x_ticlang.o \
./converter.o \
./dc_1v2.o \
./dl_uart_custom.o \
./flag.o \
./message_adc.o \
./message_log.o 

GEN_MISC_FILES += \
./device.cmd.genlibs \
./ti_msp_dl_config.h \
./Event.dot 

OBJS__QUOTED += \
"adc_to_uart.o" \
"ti_msp_dl_config.o" \
"startup_mspm0g350x_ticlang.o" \
"converter.o" \
"dc_1v2.o" \
"dl_uart_custom.o" \
"flag.o" \
"message_adc.o" \
"message_log.o" 

GEN_MISC_FILES__QUOTED += \
"device.cmd.genlibs" \
"ti_msp_dl_config.h" \
"Event.dot" 

C_DEPS__QUOTED += \
"adc_to_uart.d" \
"ti_msp_dl_config.d" \
"startup_mspm0g350x_ticlang.d" \
"converter.d" \
"dc_1v2.d" \
"dl_uart_custom.d" \
"flag.d" \
"message_adc.d" \
"message_log.d" 

GEN_FILES__QUOTED += \
"device_linker.cmd" \
"device.opt" \
"ti_msp_dl_config.c" 

C_SRCS__QUOTED += \
"../adc_to_uart.c" \
"./ti_msp_dl_config.c" \
"C:/ti/mspm0_sdk_2_00_01_00/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0g350x_ticlang.c" \
"../converter.c" \
"../dc_1v2.c" \
"../dl_uart_custom.c" \
"../flag.c" \
"../message_adc.c" \
"../message_log.c" 

SYSCFG_SRCS__QUOTED += \
"../adc_to_uart.syscfg" 


