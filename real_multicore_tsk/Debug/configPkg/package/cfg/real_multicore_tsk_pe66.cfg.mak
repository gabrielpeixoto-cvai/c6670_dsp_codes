# invoke SourceDir generated makefile for real_multicore_tsk.pe66
real_multicore_tsk.pe66: .libraries,real_multicore_tsk.pe66
.libraries,real_multicore_tsk.pe66: package/cfg/real_multicore_tsk_pe66.xdl
	$(MAKE) -f /home/gaburiero/workspace_dsp/real_multicore_tsk/src/makefile.libs

clean::
	$(MAKE) -f /home/gaburiero/workspace_dsp/real_multicore_tsk/src/makefile.libs clean

