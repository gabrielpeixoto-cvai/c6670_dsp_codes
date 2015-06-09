################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../transport_linker.cmd 

CFG_SRCS += \
../transport_t.cfg 

C_SRCS += \
../cppi_device.c \
../main_qmss.c \
../multicore_osal.c \
../qmss_device.c 

OBJS += \
./cppi_device.obj \
./main_qmss.obj \
./multicore_osal.obj \
./qmss_device.obj 

C_DEPS += \
./cppi_device.pp \
./main_qmss.pp \
./multicore_osal.pp \
./qmss_device.pp 

GEN_MISC_DIRS += \
./configPkg/ 

GEN_CMDS += \
./configPkg/linker.cmd 

GEN_OPTS += \
./configPkg/compiler.opt 

GEN_FILES += \
./configPkg/linker.cmd \
./configPkg/compiler.opt 

GEN_FILES__QUOTED += \
"configPkg/linker.cmd" \
"configPkg/compiler.opt" 

GEN_MISC_DIRS__QUOTED += \
"configPkg/" 

C_DEPS__QUOTED += \
"cppi_device.pp" \
"main_qmss.pp" \
"multicore_osal.pp" \
"qmss_device.pp" 

OBJS__QUOTED += \
"cppi_device.obj" \
"main_qmss.obj" \
"multicore_osal.obj" \
"qmss_device.obj" 

C_SRCS__QUOTED += \
"../cppi_device.c" \
"../main_qmss.c" \
"../multicore_osal.c" \
"../qmss_device.c" 

GEN_CMDS__FLAG += \
-l"./configPkg/linker.cmd" 

GEN_OPTS__FLAG += \
--cmd_file="./configPkg/compiler.opt" 


