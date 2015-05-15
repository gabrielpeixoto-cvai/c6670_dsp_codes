## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,e66 linker.cmd package/cfg/core_print_pe66.oe66

linker.cmd: package/cfg/core_print_pe66.xdl
	$(SED) 's"^\"\(package/cfg/core_print_pe66cfg.cmd\)\"$""\"/home/gaburiero/workspace_dsp/core_print/Debug/configPkg/\1\""' package/cfg/core_print_pe66.xdl > $@
