#
#  Do not edit this file.  This file is generated from 
#  package.bld.  Any modifications to this file will be 
#  overwritten whenever makefiles are re-generated.
#

unexport MAKEFILE_LIST
MK_NOGENDEPS := $(filter clean,$(MAKECMDGOALS))
override PKGDIR = configPkg
XDCINCS = -I. -I$(strip $(subst ;, -I,$(subst $(space),\$(space),$(XPKGPATH))))
XDCCFGDIR = package/cfg/

#
# The following dependencies ensure package.mak is rebuilt
# in the event that some included BOM script changes.
#
ifneq (clean,$(MAKECMDGOALS))
/home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/utils.js:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/utils.js
/home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/xdc.tci:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/xdc.tci
/home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/template.xs:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/template.xs
/home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/om2.xs:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/om2.xs
/home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/xmlgen.xs:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/xmlgen.xs
/home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/xmlgen2.xs:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/xmlgen2.xs
/home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/Warnings.xs:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/Warnings.xs
/home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/IPackage.xs:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/IPackage.xs
/home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/package.xs:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/package.xs
/home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/services/global/Clock.xs:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/services/global/Clock.xs
/home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/services/global/Trace.xs:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/services/global/Trace.xs
/home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/bld/bld.js:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/bld/bld.js
/home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/bld/BuildEnvironment.xs:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/bld/BuildEnvironment.xs
/home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/bld/PackageContents.xs:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/bld/PackageContents.xs
/home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/bld/_gen.xs:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/bld/_gen.xs
/home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/bld/Library.xs:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/bld/Library.xs
/home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/bld/Executable.xs:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/bld/Executable.xs
/home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/bld/Repository.xs:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/bld/Repository.xs
/home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/bld/Configuration.xs:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/bld/Configuration.xs
/home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/bld/Script.xs:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/bld/Script.xs
/home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/bld/Manifest.xs:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/bld/Manifest.xs
/home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/bld/Utils.xs:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/bld/Utils.xs
/home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/bld/ITarget.xs:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/bld/ITarget.xs
/home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/bld/ITarget2.xs:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/bld/ITarget2.xs
/home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/bld/ITarget3.xs:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/bld/ITarget3.xs
/home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/bld/ITargetFilter.xs:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/bld/ITargetFilter.xs
/home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/bld/package.xs:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/bld/package.xs
package.mak: config.bld
/home/gaburiero/ti/xdctools_3_25_03_72/packages/ti/targets/ITarget.xs:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/ti/targets/ITarget.xs
/home/gaburiero/ti/xdctools_3_25_03_72/packages/ti/targets/C28_large.xs:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/ti/targets/C28_large.xs
/home/gaburiero/ti/xdctools_3_25_03_72/packages/ti/targets/C28_float.xs:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/ti/targets/C28_float.xs
/home/gaburiero/ti/xdctools_3_25_03_72/packages/ti/targets/package.xs:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/ti/targets/package.xs
/home/gaburiero/ti/xdctools_3_25_03_72/packages/ti/targets/elf/ITarget.xs:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/ti/targets/elf/ITarget.xs
/home/gaburiero/ti/xdctools_3_25_03_72/packages/ti/targets/elf/package.xs:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/ti/targets/elf/package.xs
package.mak: package.bld
/home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/tools/configuro/template/compiler.opt.xdt:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/tools/configuro/template/compiler.opt.xdt
/home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/services/io/File.xs:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/services/io/File.xs
/home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/services/io/package.xs:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/services/io/package.xs
/home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/tools/configuro/template/compiler.defs.xdt:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/tools/configuro/template/compiler.defs.xdt
/home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/tools/configuro/template/custom.mak.exe.xdt:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/tools/configuro/template/custom.mak.exe.xdt
/home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/tools/configuro/template/package.xs.xdt:
package.mak: /home/gaburiero/ti/xdctools_3_25_03_72/packages/xdc/tools/configuro/template/package.xs.xdt
endif

ti.targets.elf.C66.rootDir ?= /home/gaburiero/ti/ti-cgt-c6000_8.0.1
ti.targets.elf.packageBase ?= /home/gaburiero/ti/xdctools_3_25_03_72/packages/ti/targets/elf/
.PRECIOUS: $(XDCCFGDIR)/%.oe66
.PHONY: all,e66 .dlls,e66 .executables,e66 test,e66
all,e66: .executables,e66
.executables,e66: .libraries,e66
.executables,e66: .dlls,e66
.dlls,e66: .libraries,e66
.libraries,e66: .interfaces
	@$(RM) $@
	@$(TOUCH) "$@"

.help::
	@$(ECHO) xdc test,e66
	@$(ECHO) xdc .executables,e66
	@$(ECHO) xdc .libraries,e66
	@$(ECHO) xdc .dlls,e66


all: .executables 
.executables: .libraries .dlls
.libraries: .interfaces

PKGCFGS := $(wildcard package.xs) package/build.cfg
.interfaces: package/package.xdc.inc package/package.defs.h package.xdc $(PKGCFGS)

-include package/package.xdc.dep
package/%.xdc.inc package/%_configPkg.c package/%.defs.h: %.xdc $(PKGCFGS)
	@$(MSG) generating interfaces for package configPkg" (because $@ is older than $(firstword $?))" ...
	$(XSRUN) -f xdc/services/intern/cmd/build.xs $(MK_IDLOPTS) -m package/package.xdc.dep -i package/package.xdc.inc package.xdc

.dlls,e66 .dlls: dct_multicore.pe66

-include package/cfg/dct_multicore_pe66.mak
-include package/cfg/dct_multicore_pe66.cfg.mak
ifeq (,$(MK_NOGENDEPS))
-include package/cfg/dct_multicore_pe66.dep
endif
dct_multicore.pe66: package/cfg/dct_multicore_pe66.xdl
	@


ifeq (,$(wildcard .libraries,e66))
dct_multicore.pe66 package/cfg/dct_multicore_pe66.c: .libraries,e66
endif

package/cfg/dct_multicore_pe66.c package/cfg/dct_multicore_pe66.h package/cfg/dct_multicore_pe66.xdl: override _PROG_NAME := dct_multicore.xe66
package/cfg/dct_multicore_pe66.c: package/cfg/dct_multicore_pe66.cfg

clean:: clean,e66
	-$(RM) package/cfg/dct_multicore_pe66.cfg
	-$(RM) package/cfg/dct_multicore_pe66.dep
	-$(RM) package/cfg/dct_multicore_pe66.c
	-$(RM) package/cfg/dct_multicore_pe66.xdc.inc

clean,e66::
	-$(RM) dct_multicore.pe66
.executables,e66 .executables: dct_multicore.xe66

dct_multicore.xe66: |dct_multicore.pe66

-include package/cfg/dct_multicore.xe66.mak
dct_multicore.xe66: package/cfg/dct_multicore_pe66.oe66 
	$(RM) $@
	@$(MSG) lnke66 $@ ...
	$(RM) $(XDCCFGDIR)/$@.map
	$(ti.targets.elf.C66.rootDir)/bin/lnk6x -w -q -u _c_int00 -fs $(XDCCFGDIR)$(dir $@)  -q -o $@ package/cfg/dct_multicore_pe66.oe66   package/cfg/dct_multicore_pe66.xdl --abi=eabi -c -m $(XDCCFGDIR)/$@.map -l $(ti.targets.elf.C66.rootDir)/lib/libc.a
	
dct_multicore.xe66: export C_DIR=
dct_multicore.xe66: PATH:=$(ti.targets.elf.C66.rootDir)/bin/:$(PATH)

dct_multicore.test test,e66 test: dct_multicore.xe66.test

dct_multicore.xe66.test:: dct_multicore.xe66
ifeq (,$(_TESTLEVEL))
	@$(MAKE) -R -r --no-print-directory -f $(XDCROOT)/packages/xdc/bld/xdc.mak _TESTLEVEL=1 dct_multicore.xe66.test
else
	@$(MSG) running $<  ...
	$(call EXEC.dct_multicore.xe66, ) 
endif

clean,e66::
	-$(RM) .tmp,dct_multicore.xe66,0,*


clean:: clean,e66

clean,e66::
	-$(RM) dct_multicore.xe66
clean:: 
	-$(RM) package/cfg/dct_multicore_pe66.pjt
%,copy:
	@$(if $<,,$(MSG) don\'t know how to build $*; exit 1)
	@$(MSG) cp $< $@
	$(RM) $@
	$(CP) $< $@
dct_multicore_pe66.oe66,copy : package/cfg/dct_multicore_pe66.oe66
dct_multicore_pe66.se66,copy : package/cfg/dct_multicore_pe66.se66

$(XDCCFGDIR)%.c $(XDCCFGDIR)%.h $(XDCCFGDIR)%.xdl: $(XDCCFGDIR)%.cfg .interfaces $(XDCROOT)/packages/xdc/cfg/Main.xs
	@$(MSG) "configuring $(_PROG_NAME) from $< ..."
	$(CONFIG) $(_PROG_XSOPTS) xdc.cfg $(_PROG_NAME) $(XDCCFGDIR)$*.cfg $(XDCCFGDIR)$*

.PHONY: release,configPkg
configPkg.tar: package/package.bld.xml
configPkg.tar: package/build.cfg
configPkg.tar: package/package.xdc.inc
ifeq (,$(MK_NOGENDEPS))
-include package/rel/configPkg.tar.dep
endif
package/rel/configPkg/configPkg/package/package.rel.xml: .force
	@$(MSG) generating external release references $@ ...
	$(XS) $(JSENV) -f $(XDCROOT)/packages/xdc/bld/rel.js $(MK_RELOPTS) . $@

configPkg.tar: package/rel/configPkg.xdc.inc package/rel/configPkg/configPkg/package/package.rel.xml
	@$(MSG) making release file $@ "(because of $(firstword $?))" ...
	-$(RM) $@
	$(call MKRELTAR,package/rel/configPkg.xdc.inc,package/rel/configPkg.tar.dep)


release release,configPkg: all configPkg.tar
clean:: .clean
	-$(RM) configPkg.tar
	-$(RM) package/rel/configPkg.xdc.inc
	-$(RM) package/rel/configPkg.tar.dep

clean:: .clean
	-$(RM) .libraries .libraries,*
clean:: 
	-$(RM) .dlls .dlls,*
#
# The following clean rule removes user specified
# generated files or directories.
#

ifneq (clean,$(MAKECMDGOALS))
ifeq (,$(wildcard package))
    $(shell $(MKDIR) package)
endif
ifeq (,$(wildcard package/cfg))
    $(shell $(MKDIR) package/cfg)
endif
ifeq (,$(wildcard package/lib))
    $(shell $(MKDIR) package/lib)
endif
ifeq (,$(wildcard package/rel))
    $(shell $(MKDIR) package/rel)
endif
ifeq (,$(wildcard package/internal))
    $(shell $(MKDIR) package/internal)
endif
endif
clean::
	-$(RMDIR) package

include custom.mak
clean:: 
	-$(RM) package/configPkg.pjt
