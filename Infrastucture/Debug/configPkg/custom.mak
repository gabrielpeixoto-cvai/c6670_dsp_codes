## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,e66 linker.cmd package/cfg/infrastructure_multicoremode_pe66.oe66

linker.cmd: package/cfg/infrastructure_multicoremode_pe66.xdl
	$(SED) 's"^\"\(package/cfg/infrastructure_multicoremode_pe66cfg.cmd\)\"$""\"/home/gaburiero/workspace_dsp/Infrastucture/Debug/configPkg/\1\""' package/cfg/infrastructure_multicoremode_pe66.xdl > $@
