################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../infrastructure_multicoremode.cmd 

CFG_SRCS += \
../infrastructure_multicoremode.cfg 

C_SRCS += \
../infrastructure_multicoremode.c \
../infrastructure_multicoreosal.c 

OBJS += \
./infrastructure_multicoremode.obj \
./infrastructure_multicoreosal.obj 

C_DEPS += \
./infrastructure_multicoremode.pp \
./infrastructure_multicoreosal.pp 

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
"infrastructure_multicoremode.pp" \
"infrastructure_multicoreosal.pp" 

OBJS__QUOTED += \
"infrastructure_multicoremode.obj" \
"infrastructure_multicoreosal.obj" 

C_SRCS__QUOTED += \
"../infrastructure_multicoremode.c" \
"../infrastructure_multicoreosal.c" 

GEN_CMDS__FLAG += \
-l"./configPkg/linker.cmd" 

GEN_OPTS__FLAG += \
--cmd_file="./configPkg/compiler.opt" 


