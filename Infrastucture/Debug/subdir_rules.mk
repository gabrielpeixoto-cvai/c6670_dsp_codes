################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
infrastructure_multicoremode.obj: ../infrastructure_multicoremode.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"/home/gaburiero/ti/ti-cgt-c6000_8.0.1/bin/cl6x" -mv6600 --abi=eabi --include_path="/home/gaburiero/ti/ti-cgt-c6000_8.0.1/include" --include_path="/home/gaburiero/ti/pdk_C6670_1_1_2_6/packages/ti/drv/qmss" --include_path="/home/gaburiero/ti/pdk_C6670_1_1_2_6/packages/ti/drv/cppi" --include_path="/home/gaburiero/ti/pdk_C6670_1_1_2_6/packages/ti/csl/src/intc" -g --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="infrastructure_multicoremode.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

configPkg/linker.cmd: ../infrastructure_multicoremode.cfg
	@echo 'Building file: $<'
	@echo 'Invoking: XDCtools'
	"/home/gaburiero/ti/xdctools_3_25_03_72/xs" --xdcpath="/home/gaburiero/ti/bios_6_35_04_50/packages;/home/gaburiero/ti/ipc_1_25_00_04/packages;/home/gaburiero/ti/pdk_C6657_1_1_2_6/packages;/home/gaburiero/ti/mcsdk_2_01_02_06/demos;/home/gaburiero/ti/uia_1_03_01_08/packages;/home/gaburiero/ti/ccsv6/ccs_base;" xdc.tools.configuro -o configPkg -t ti.targets.elf.C66 -p ti.platforms.evm6670 -r release -c "/home/gaburiero/ti/ti-cgt-c6000_8.0.1" --compileOptions "-g --optimize_with_debug" "$<"
	@echo 'Finished building: $<'
	@echo ' '

configPkg/compiler.opt: | configPkg/linker.cmd
configPkg/: | configPkg/linker.cmd

infrastructure_multicoreosal.obj: ../infrastructure_multicoreosal.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"/home/gaburiero/ti/ti-cgt-c6000_8.0.1/bin/cl6x" -mv6600 --abi=eabi --include_path="/home/gaburiero/ti/ti-cgt-c6000_8.0.1/include" --include_path="/home/gaburiero/ti/pdk_C6670_1_1_2_6/packages/ti/drv/qmss" --include_path="/home/gaburiero/ti/pdk_C6670_1_1_2_6/packages/ti/drv/cppi" --include_path="/home/gaburiero/ti/pdk_C6670_1_1_2_6/packages/ti/csl/src/intc" -g --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="infrastructure_multicoreosal.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '


