#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = /home/gaburiero/ti/bios_6_35_04_50/packages;/home/gaburiero/ti/ctoolslib_1_0_0_2/packages/ti;/home/gaburiero/ti/edma3_lld_02_11_05_02/packages;/home/gaburiero/ti/ipc_1_25_00_04/packages;/home/gaburiero/ti/mcsdk_2_01_02_06/demos;/home/gaburiero/ti/pdk_C6670_1_1_2_6/packages;/home/gaburiero/ti/uia_1_03_01_08/packages;/home/gaburiero/ti/ccsv6/ccs_base
override XDCROOT = /home/gaburiero/ti/xdctools_3_25_03_72
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = /home/gaburiero/ti/bios_6_35_04_50/packages;/home/gaburiero/ti/ctoolslib_1_0_0_2/packages/ti;/home/gaburiero/ti/edma3_lld_02_11_05_02/packages;/home/gaburiero/ti/ipc_1_25_00_04/packages;/home/gaburiero/ti/mcsdk_2_01_02_06/demos;/home/gaburiero/ti/pdk_C6670_1_1_2_6/packages;/home/gaburiero/ti/uia_1_03_01_08/packages;/home/gaburiero/ti/ccsv6/ccs_base;/home/gaburiero/ti/xdctools_3_25_03_72/packages;..
HOSTOS = Linux
endif
