#******************************************************************************
#
# Copyright (C) u-blox AG 
# u-blox AG, Thalwil, Switzerland
# 
# All rights reserved.
# 
# Permission to use, copy, modify, and distribute this software for any
# purpose without fee is hereby granted, provided that this entire notice
# is included in all copies of any software which is or includes a copy
# or modification of this software and in all copies of the supporting
# documentation for such software.
# 
# THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
# WARRANTY. IN PARTICULAR, NEITHER THE AUTHOR NOR U-BLOX MAKES ANY
# REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY
# OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.
#
#******************************************************************************


#******************************************************************************/
#
#	This is the makefile for Lint checks 
#
#   Dependencies: 
#     - Lint installed 
#     - on linux, tools environment only accessible through bronto
#     - on windows, no tests have been done yet
#   If this does not work take a look also at the u-blox wiki page
#   LintForAndroidDriver
#   
#   This makefile does the following steps to process files
#   1. it finds source and header files
#   2. it generates individual makefiles for each source file that takes
#      first-level include headers into consideration
#   3. uses these generated makefiles to check the files in unit tests
#   4. perform a "lint linking" step to check the driver as a whole
#   
#   
#  
#/*******************************************************************************
# * $Id: Makefile miklos.kirilly $
# * $HeadURL: $
# ******************************************************************************/

PLATFORM_SDK_VERSION := 15

# source files
CPP_EXT := .cpp
HEADER_EXT := .h
# ASN.1 header folder contains generated files, we don't want to check them,
# but headers are used in the lint check
#SOURCE_FOLDERS := . ./supl 
SOURCE_FOLDERS := . ./supl ./parser

# Lint options file used as a command line option for Lint
LINT_OPTSFILE := ./lint/options.lnt
# Lint prequisite files not directly put on the Lint command line but instead referenced by the LINT_OPTSFILE
# They are not used directly in this makefile only as dependencies.
LINT_SETTINGS_FILES := ./lint/gcc-include-path.lnt ./lint/size-options.lnt ./lint/co-gcc.lnt
# defined symbols for the compiler
LINT_GEN_HEADERS := ./lint/lint_cmac.h ./lint/lint_cppmac.h

# various flags
# flags passed to the C preprocessor, defined in the module makefile Android.mk
MODULE_FLAGS:=-DNDEBUG -UDEBUG -DSUPL_ENABLED -DPLARFORM_SDK_VERSION=$(PLATFORM_SDK_VERSION) -DUNIX_API -DANDROID_BUILD -DUDP_SERVER_PORT=46434
# referenced library header search paths
LIB_INCLUDE_FOLDERS := ./supl/asn1c_header external external/openssl external/openssl/include
# all include folders specific for this module
MODULE_INCLUDE_FOLDERS:= $(SOURCE_FOLDERS) $(LIB_INCLUDE_FOLDERS)

# Lint options contain the option file, include search dirs, always included header and a few flags
LINT_OPTS := $(LINT_OPTSFILE) $(addprefix -i, $(MODULE_INCLUDE_FOLDERS)) ${addsuffix )', ${addprefix '+libdir(,$(LIB_INCLUDE_FOLDERS) }}\
	 '-header(system/core/include/arch/linux-arm/AndroidConfig.h)' '+libh(system/core/include/arch/linux-arm/AndroidConfig.h)' $(MODULE_FLAGS) +v -zero -b 

# programs used in this makefile
AWK   := awk
FIND  := find
CAT   := cat
GREP  := grep
SED   := sed
SORT  := sort
TR    := tr
UNIQ  := uniq
PERL  := perl
MKDIR := mkdir
LINT  := /home/foni/bin/flint
#LINT  := /u-blox/pcsoft/install/flexelint/flint

# intermediate files' folder
INTERMEDIATES := ./intermediates
OUT_DIR:= ./out

# search for source files
CPP_FILES := $(subst ./,,$(foreach folder,$(SOURCE_FOLDERS), $(shell ls $(folder)/*$(CPP_EXT) 2> /dev/null)) )
C_FILES   := $(subst ./,,$(foreach folder,$(SOURCE_FOLDERS), $(shell ls $(folder)/*.c 2> /dev/null )) )
SOURCE_FILES := $(subst ./,,$(C_FILES) $(CPP_FILES))
HEADER_FILES := $(foreach folder,$(SOURCE_FOLDERS), $(shell ls $(folder)/*.h))
HEADER_LISTS := $(addsuffix .header_list, $(addprefix $(INTERMEDIATES)/, $(subst ./,,  $(SOURCE_FILES)) ) )
INDIV_LINT_MAKEFILES :=  $(addsuffix .lintmk, $(addprefix $(INTERMEDIATES)/lint/, $(subst ./,,  $(SOURCE_FILES)) ) )

# lint intermediate files
LOBS := $(addsuffix .lob, $(addprefix $(INTERMEDIATES)/lint/, $(subst ./,, $(SOURCE_FILES) ) ) )

# include makefiles that hold the make rules for the .lob files
-include $(INDIV_LINT_MAKEFILES)

#################################################################################
# Targets
#################################################################################
# print help message
.PHONY: help
help:
	@echo "This makefile is only intended for Lint checks. Targets:"
	@echo "        - lint: perform checks"
	@echo "        - clean-lint: clean up all lint related output"
	@echo "        - update_lint_makefiles: update make rules for sources."
	@echo "           Note that this is performed automatically for either former target."
	@echo "To build the Android driver do it in the regular Android way."
	@echo "Currently this makefile has only been tested on bronto/Thalwil."

# Lint
.PHONY: lint

# Lint linking part
# touch command issued to create lintlink.log even if logfiles are empty
lint: $(LOBS) Makefile $(LINT_OPTSFILE) $(LINT_SETTINGS_FILES) $(LINT_GEN_HEADERS)
	@$(MKDIR) -p $(OUT_DIR)
	@echo Invoking: Lint Linker
	@$(LINT) $(LINT_OPTS) "-os($(INTERMEDIATES)/lint/lintlink.log)" $(LOBS)
	@$(CAT) $(LOBS:%.lob=%.log) $(INTERMEDIATES)/lint/lintlink.log > $(OUT_DIR)/lint.log
	@echo ========================================
	@echo "Final Lint output: $(OUT_DIR)/lint.log"

# Generate makefiles for each source file that contain the dependencies inside the projects
# and the definition of the lint recipe.
# Note that since these makefiles are included into this one they are always built whenever
# any target is executed.
.PHONY: update_lint_makefiles
update_lint_makefiles: $(INDIV_LINT_MAKEFILES)
	@echo '--- individual makefiles updated ---'

# dependencies of .lobs are defined here as:
#   - the source file
#   - the first level included headers
#   - the lit options file and files included therein
$(INDIV_LINT_MAKEFILES): $(INTERMEDIATES)/lint/%.lintmk: % Makefile
	@$(MKDIR) -p $(dir $@) 
	@echo '# automatically generated by the root makefile for lint compilation using make update_lint_makefile' > $@
# dependencies: C/C++ source file + header files that are in the user-controlled folders
# To avoid looking for library headers only header files from these folders are listed here
	@echo '$(basename $@).lob: $< $$(filter $$(HEADER_FILES),\' >> $@
# parse C/C++ file to find include directives and isolate the header file names
	@$(GREP) '^#include' $< | $(TR) -d '<>"'  | $(AWK) '{ print $$2 ; }' | $(SORT) | $(UNIQ) | $(TR) '\r\n' ' ' >> $@ 
	@echo ')\'  >> $@
# add dependency on lint options
	@echo '$$(LINT_OPTSFILE) $$(LINT_SETTINGS_FILES) $$(LINT_GEN_HEADERS)' >> $@
	@echo '	@$$(MKDIR) -p $(dir $@)' >> $@
	@echo '	@echo Compiling file with lint: $<' >>$@
	@echo '	$$(LINT) $$(LINT_OPTS) -u "-oo($(@:%.lintmk=%.lob))" "-os($(@:%.lintmk=%.log))" "+program_info(output_prefix=$(@:%.lintmk=%.)" "+stack(&file=$(@:%.lintmk=%.stack.txt))" "-e974" "$<"' >> $@
	@echo updated $@

.PHONY: clean-lint
clean-lint:
	rm -rf $(INTERMEDIATES)/lint
	rm -f $(OUT_DIR)/lint.log
