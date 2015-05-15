# invoke SourceDir generated makefile for core_multitask.pe66
core_multitask.pe66: .libraries,core_multitask.pe66
.libraries,core_multitask.pe66: package/cfg/core_multitask_pe66.xdl
	$(MAKE) -f /home/gaburiero/workspace_dsp/core_multi_task/src/makefile.libs

clean::
	$(MAKE) -f /home/gaburiero/workspace_dsp/core_multi_task/src/makefile.libs clean

