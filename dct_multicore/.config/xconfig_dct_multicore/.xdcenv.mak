#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = /home/gaburiero/ti/bios_6_35_04_50/packages;/home/gaburiero/ti/uia_1_03_01_08/packages;/home/gaburiero/ti/ccsv6/ccs_base;/home/gaburiero/workspace_dsp/dct_multicore/.config
override XDCROOT = /home/gaburiero/ti/xdctools_3_25_03_72
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = /home/gaburiero/ti/bios_6_35_04_50/packages;/home/gaburiero/ti/uia_1_03_01_08/packages;/home/gaburiero/ti/ccsv6/ccs_base;/home/gaburiero/workspace_dsp/dct_multicore/.config;/home/gaburiero/ti/xdctools_3_25_03_72/packages;..
HOSTOS = Linux
endif
