# invoke SourceDir generated makefile for core_print.pe66
core_print.pe66: .libraries,core_print.pe66
.libraries,core_print.pe66: package/cfg/core_print_pe66.xdl
	$(MAKE) -f /home/gaburiero/workspace_dsp/core_print/src/makefile.libs

clean::
	$(MAKE) -f /home/gaburiero/workspace_dsp/core_print/src/makefile.libs clean

