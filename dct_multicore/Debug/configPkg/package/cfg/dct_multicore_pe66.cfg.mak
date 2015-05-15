# invoke SourceDir generated makefile for dct_multicore.pe66
dct_multicore.pe66: .libraries,dct_multicore.pe66
.libraries,dct_multicore.pe66: package/cfg/dct_multicore_pe66.xdl
	$(MAKE) -f /home/gaburiero/workspace_dsp/dct_multicore/src/makefile.libs

clean::
	$(MAKE) -f /home/gaburiero/workspace_dsp/dct_multicore/src/makefile.libs clean

