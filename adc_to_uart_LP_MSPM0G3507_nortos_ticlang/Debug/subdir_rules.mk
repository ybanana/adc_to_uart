################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccstheia140/ccs/tools/compiler/ti-cgt-armllvm_3.2.2.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"C:/Users/ehuiyue/workspace_ccstheia/adc_to_uart_LP_MSPM0G3507_nortos_ticlang" -I"C:/Users/ehuiyue/workspace_ccstheia/adc_to_uart_LP_MSPM0G3507_nortos_ticlang/Debug" -I"C:/ti/mspm0_sdk_2_00_01_00/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_00_01_00/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

adc_to_uart.opt: ../adc_to_uart.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/ccstheia140/ccs/utils/sysconfig_1.20.0/sysconfig_cli.bat" --script "C:/Users/ehuiyue/workspace_ccstheia/adc_to_uart_LP_MSPM0G3507_nortos_ticlang/adc_to_uart.syscfg" -o "." -s "C:/ti/mspm0_sdk_2_00_01_00/.metadata/product.json" --compiler ticlang
	@echo 'Finished building: "$<"'
	@echo ' '


