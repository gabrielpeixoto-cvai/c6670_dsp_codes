## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,e66 linker.cmd package/cfg/core_multitask_pe66.oe66

linker.cmd: package/cfg/core_multitask_pe66.xdl
	$(SED) 's"^\"\(package/cfg/core_multitask_pe66cfg.cmd\)\"$""\"/home/gaburiero/workspace_dsp/core_multi_task/Debug/configPkg/\1\""' package/cfg/core_multitask_pe66.xdl > $@
