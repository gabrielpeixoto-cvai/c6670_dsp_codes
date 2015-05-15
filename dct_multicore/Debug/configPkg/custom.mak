## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,e66 linker.cmd package/cfg/dct_multicore_pe66.oe66

linker.cmd: package/cfg/dct_multicore_pe66.xdl
	$(SED) 's"^\"\(package/cfg/dct_multicore_pe66cfg.cmd\)\"$""\"/home/gaburiero/workspace_dsp/dct_multicore/Debug/configPkg/\1\""' package/cfg/dct_multicore_pe66.xdl > $@
