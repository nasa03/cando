


ifneq ($(CLASP_INTERNAL_BUILD_TARGET_DIR),)
  CLASP_BOEHM = $(CLASP_INTERNAL_BUILD_TARGET_DIR)/$(EXECUTABLE_DIR)/clasp_boehm_o
  CLASP_MPS = $(CLASP_INTERNAL_BUILD_TARGET_DIR)/$(EXECUTABLE_DIR)/clasp_mps_o
else
  CLASP_BOEHM = clasp_boehm_o
  CLASP_MPS = clasp_mps_o
endif

export CLASP_APP_RESOURCES_DIR=$(CLASP_INTERNAL_BUILD_TARGET_DIR)/Contents/Resources



all:
	make boehm
	make mps

application:
	cp -R $(CLASP_LISP_SOURCE_DIR)/build $(CLASP_APP_RESOURCES_DIR)/lisp/
	cp -R $(CLASP_INTERNAL_BUILD_TARGET_DIR)/* $(CLASP_APP_DIR)

boehm-cleavir:
	make boehm
	make cleavir

cclasp:
	clasp_boehm_o  -l "buildCleavirClasp.lsp"

bclasp-to-cclasp:
	clasp_boehm_o  -l "buildBclaspToCclasp.lsp"


#	make clasp-gc-interface



boehm:
	make min-boehm
	make full-boehm

boehm-recompile:
	make min-boehm-recompile
	make full-boehm

min-boehm:
	$(CLASP_BOEHM) -I -f ecl-min -l "buildClaspMinBoot.lsp"

min-boehm-recompile:
	$(CLASP_BOEHM) -f ecl-min -l "buildClaspMinRecompile.lsp"

full-boehm:
	$(CLASP_BOEHM) -f ecl-min -l "buildClasp.lsp"


mps:
	make min-mps
	make full-mps

mps-recompile:
	make min-mps-recompile
	make full-mps

min-mps:
	$(CLASP_MPS) -I -f ecl-min -l "buildClaspMinMps.lsp"

min-mps-recompile:
	$(CLASP_MPS) -f ecl-min -l "buildClaspMinRecompile.lsp"

full-mps:
	$(CLASP_MPS) -f ecl-min -l "buildClasp.lsp"


short-compile-mps:
	time $(CLASP_MPS) -f ecl-min -l "shortCompile.lsp"

short-compile-boehm:
	time $(CLASP_BOEHM) -f ecl-min -l "shortCompile.lsp"



clasp-gc-interface:
	$(CLASP_BOEHM) -l "buildClaspGC.lsp"

compile-commands:
	-bjam bundle release mps-prep -n -a >bjam.out
	-install -d $(CLASP_APP_RESOURCES_DIR)/build-databases
	python ../common/buildJSON.py clang bjam.out `$(CLASP_BOEHM) -N -e "(progn (format t \"~a\"(namestring (translate-logical-pathname (ensure-directories-exist #P\"APP-RESOURCES:build-databases;clasp_compile_commands.json\")))) (quit))"` # $(CLASP_APP_RESOURCES_DIR)/build-databases/clasp_compile_commands.json


run-clang-check:
	clang-check -p . main.cc

run-clasp:
	clasp_d clang0.lsp


mps-interface:
	@echo Building mps-prep
	bjam -j$(PJOBS) bundle release mps-prep
	make boehm
	make compile-commands
	@echo Running parallel search and generating code when done
	clasp_boehm_o -l "sys:mps-interface.lsp" -e "(parallel-search-all-then-generate-code-and-quit)"