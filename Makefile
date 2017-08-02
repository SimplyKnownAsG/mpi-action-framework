
PROJECT:=maf

SRC_DIR=$(PROJECT)
OBJ_DIR=$(BUILD_DIR)
BIN_DIR=$(BUILD_DIR)/bin


include MakefileFunctions.in
ifeq ($(OS),Windows_NT)
	CXX=cl
	CXXFLAGS= /nologo /EHa /fp:strict /MP "/IC:\Program Files\Microsoft HPC Pack 2012\Inc" "/IC:\tools\Python27\include" /I. /c $< /Fo:$@
	LDFLAGS= /nologo /incremental:no '/libpath:C:\tools\Python27\libs' '/libpath:C:\Program Files\Microsoft HPC Pack 2012\Lib\amd64' msmpi.lib
	LDSHARED=link $(LDFLAGS) /DLL /out:$@
	LDSTATIC=lib $(LDFLAGS) /out:$@
	LDEXE=link $(LDFLAGS) /out:$@
	LIB$(PROJECT)=$(BUILD_DIR)/lib/libmaf.lib
	PY_EXT=$(BIN_DIR)/_maf.pyd
else
	CXX=mpic++
	LD=mpic++
	CXXFLAGS= -fPIC -Wall -std=c++1y -c $< -o $@ -I/usr/include/python2.7
	LDFLAGS= -fPIC
	LDSHARED=mpic++ -Wall -shared -o $@
	LDSTATIC=ar crs $@
	LDEXE=mpic++ -Wall -o $@
	LIB$(PROJECT)=$(BUILD_DIR)/lib/libmaf.a
	PY_EXT=$(BIN_DIR)/_maf.so
endif

ifeq ($(MAKECMDGOALS),debug)
	BUILD_DIR=debug
	CXXFLAGS+= -g
	LDFLAGS+= -g
else
	CXXFLAGS+= -O2
	BUILD_DIR=release
endif


SRC = $(filter-out %_wrap.cpp,$(call rwildcard, $(SRC_DIR), *.cpp)) $(SRC_DIR)/Version.cpp
HEADERS = $(filter-out %_wrap.hpp maf/maf.hpp, $(call rwildcard, $(SRC_DIR), *.hpp))
DIRECTORIES = $(sort $(dir $(SRC) $(HEADERS)))
OBJ = $(filter-out %_wrap.obj,$(patsubst %.cpp,$(OBJ_DIR)/%.obj,$(SRC)))

CPP_TEST_EXEC := $(sort $(patsubst %.cpp, $(BIN_DIR)/%.exe, $(call rwildcard, tests examples, *.cpp)))
CPP_TEST_OBJ := $(patsubst $(BIN_DIR)/%.exe,$(OBJ_DIR)/%.obj,$(CPP_TEST_EXEC))

.PHONY: test test_py test_cpp

all: release

# this must be placed after your .DEFAULT_GOAL, or you can manually state what it is
# https://www.gnu.org/software/make/manual/html_node/Special-Variables.html
$(call create_dirs,$(OBJ) $(LIB$(PROJECT)) $(CPP_TEST_EXEC) $(CPP_TEST_OBJ))

release: test examples

debug: test examples

clean:
	-$(RM) -r ./debug/ ./release/
	-$(RM) ./maf/maf.hpp
	-$(RM) ./maf/*wrap*

clobber: clean
	-git clean -fx

maf/maf.hpp: $(HEADERS)
	@echo "// this file is auto-generated, do not modify" > $@
	@$(foreach hpp,$^,echo '#include "$(hpp)"' >> $@;)
	@echo "" >> $@

maf/Version.cpp: $(filter-out %wrap.hpp %wrap.cpp %Version.cpp,$(HEADERS) $(SRC))
	@echo '#include "maf/Version.hpp"' > $@
	@echo '' >> $@
	@echo 'namespace maf {' >> $@
	@echo '    const std::string Version::git_sha1 = "'`git rev-parse HEAD`'";' >> $@
	@echo '' >> $@
	@echo '    const std::string Version::git_description = "'`git describe --always --dirty`'";' >> $@
	@echo '}' >> $@
	@echo "" >> $@

$(LIB$(PROJECT)): $(OBJ)
	$(call colorecho,$(LDSTATIC) $^)

$(OBJ_DIR)/%.obj: %.cpp %.hpp | $(TARGET_DIRS)
	$(call colorecho,$(CXX) $(CXXFLAGS) -I.)

test: test_cpp test_py

examples: cpp_examples py_examples

# cpp targets
cpp: test_cpp cpp_examples

test_cpp: $(sort $(patsubst %.cpp, $(BIN_DIR)/%.exe, $(call rwildcard, tests, *.cpp))) | $(TARGET_DIRS)
	$(foreach cpp_test_exec, $^, $(call colorecho,mpiexec -n 3 $(cpp_test_exec)))

cpp_examples: $(sort $(patsubst %.cpp, $(BIN_DIR)/%.exe, $(call rwildcard, examples, *.cpp))) | $(TARGET_DIRS)
	$(foreach cpp_test_exec, $^, $(call colorecho,mpiexec -n 3 $(cpp_test_exec)))

$(BIN_DIR)/%.exe: maf/maf.hpp $(OBJ_DIR)/%.obj $(LIB$(PROJECT)) | $(TARGET_DIRS)
	$(call colorecho,$(LDEXE) $(filter-out maf/maf.hpp,$^))

$(OBJ_DIR)/%.obj: %.cpp | $(TARGET_DIRS)
	$(call colorecho,$(CXX) $(CXXFLAGS) -I.)

# Python targets
py: test_py py_examples

test_py: PY_TEST_FILES=$(sort $(call rwildcard, tests, *.py))
test_py: $(PY_EXT)
	$(foreach pytestfile, $(PY_TEST_FILES), $(call colorecho,PYTHONPATH=$(BIN_DIR) mpiexec -n 2 python $(pytestfile)))

py_examples: PY_EXAMPLE_FILES=$(sort $(call rwildcard, examples, *.py))
py_examples: $(PY_EXT)
	$(foreach pytestfile, $(PY_EXAMPLE_FILES), $(call colorecho,PYTHONPATH=$(BIN_DIR) mpiexec -n 2 python $(pytestfile)))

$(PY_EXT): $(LIB$(PROJECT)) $(OBJ_DIR)/maf/maf_wrap.obj | $(TARGET_DIRS)
	@# $< is repeated (implicit in $^) because the symbols are only needed after maf_wrap.obj
	$(call colorecho,$(LDSHARED) $^ $<)

$(SRC_DIR)/maf_wrap.cpp $(SRC_DIR)/maf_wrap.hpp: maf.i maf/maf.hpp
	$(call colorecho,swig -python -builtin -includeall -ignoremissing -c++ -outdir $(BIN_DIR) -o $(SRC_DIR)/maf_wrap.cpp -oh $(SRC_DIR)/maf_wrap.hpp maf.i)

