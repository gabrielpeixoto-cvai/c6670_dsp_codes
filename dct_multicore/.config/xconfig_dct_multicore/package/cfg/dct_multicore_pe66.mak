#
#  Do not edit this file.  This file is generated from 
#  package.bld.  Any modifications to this file will be 
#  overwritten whenever makefiles are re-generated.
#
#  target compatibility key = ti.targets.elf.C66{1,0,8.0,1
#
ifeq (,$(MK_NOGENDEPS))
-include package/cfg/dct_multicore_pe66.oe66.dep
package/cfg/dct_multicore_pe66.oe66.dep: ;
endif

package/cfg/dct_multicore_pe66.oe66: | .interfaces
package/cfg/dct_multicore_pe66.oe66: package/cfg/dct_multicore_pe66.c package/cfg/dct_multicore_pe66.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle66 $< ...
	$(ti.targets.elf.C66.rootDir)/bin/cl6x -c  -g --optimize_with_debug -qq -pdsw225 -mo -mv6600 --abi=eabi -eo.oe66 -ea.se66  --embed_inline_assembly  -Dxdc_cfg__header__='xconfig_dct_multicore/package/cfg/dct_multicore_pe66.h'  -Dxdc_target_name__=C66 -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_8_0_1 -O2  $(XDCINCS) -I$(ti.targets.elf.C66.rootDir)/include -fs=./package/cfg -fr=./package/cfg -fc $<
	$(MKDEP) -a $@.dep -p package/cfg -s oe66 $< -C   -g --optimize_with_debug -qq -pdsw225 -mo -mv6600 --abi=eabi -eo.oe66 -ea.se66  --embed_inline_assembly  -Dxdc_cfg__header__='xconfig_dct_multicore/package/cfg/dct_multicore_pe66.h'  -Dxdc_target_name__=C66 -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_8_0_1 -O2  $(XDCINCS) -I$(ti.targets.elf.C66.rootDir)/include -fs=./package/cfg -fr=./package/cfg
	-@$(FIXDEP) $@.dep $@.dep
	
package/cfg/dct_multicore_pe66.oe66: export C_DIR=
package/cfg/dct_multicore_pe66.oe66: PATH:=$(ti.targets.elf.C66.rootDir)/bin/:$(PATH)

package/cfg/dct_multicore_pe66.se66: | .interfaces
package/cfg/dct_multicore_pe66.se66: package/cfg/dct_multicore_pe66.c package/cfg/dct_multicore_pe66.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle66 -n $< ...
	$(ti.targets.elf.C66.rootDir)/bin/cl6x -c -n -s --symdebug:none -g --optimize_with_debug -qq -pdsw225 -mv6600 --abi=eabi -eo.oe66 -ea.se66   -Dxdc_cfg__header__='xconfig_dct_multicore/package/cfg/dct_multicore_pe66.h'  -Dxdc_target_name__=C66 -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_8_0_1 -O2  $(XDCINCS) -I$(ti.targets.elf.C66.rootDir)/include -fs=./package/cfg -fr=./package/cfg -fc $<
	$(MKDEP) -a $@.dep -p package/cfg -s oe66 $< -C  -n -s --symdebug:none -g --optimize_with_debug -qq -pdsw225 -mv6600 --abi=eabi -eo.oe66 -ea.se66   -Dxdc_cfg__header__='xconfig_dct_multicore/package/cfg/dct_multicore_pe66.h'  -Dxdc_target_name__=C66 -Dxdc_target_types__=ti/targets/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_8_0_1 -O2  $(XDCINCS) -I$(ti.targets.elf.C66.rootDir)/include -fs=./package/cfg -fr=./package/cfg
	-@$(FIXDEP) $@.dep $@.dep
	
package/cfg/dct_multicore_pe66.se66: export C_DIR=
package/cfg/dct_multicore_pe66.se66: PATH:=$(ti.targets.elf.C66.rootDir)/bin/:$(PATH)

clean,e66 ::
	-$(RM) package/cfg/dct_multicore_pe66.oe66
	-$(RM) package/cfg/dct_multicore_pe66.se66

dct_multicore.pe66: package/cfg/dct_multicore_pe66.oe66 package/cfg/dct_multicore_pe66.mak

clean::
	-$(RM) package/cfg/dct_multicore_pe66.mak
