## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,e66 linker.cmd package/cfg/real_multicore_tsk_pe66.oe66

linker.cmd: package/cfg/real_multicore_tsk_pe66.xdl
	$(SED) 's"^\"\(package/cfg/real_multicore_tsk_pe66cfg.cmd\)\"$""\"/home/gaburiero/workspace_dsp/real_multicore_tsk/Debug/configPkg/\1\""' package/cfg/real_multicore_tsk_pe66.xdl > $@
