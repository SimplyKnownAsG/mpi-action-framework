
ifeq ($(OS),Windows_NT)
	COMPILER=cl
	LINKER=link
	CCFLAGS += -D WIN32
	COMPILER_FLAGS= /nologo /fp:strict /MP "/IC:\Program Files\Microsoft HPC Pack 2012\Inc" "/IC:\Python27\include" /I.-c $< /Fo:$@
	LINK_FLAGS= /DLL /out:$(PY_EXT) /nologo /incremental:no '/libpath:C:\Python27\libs' '/libpath:C:\Program Files\Microsoft HPC Pack 2012\Lib\amd64' msmpi.lib
	PY_EXT=_maf.pyd
else
	COMPILER=mpic++
	LINKER=mpic++
	COMPILER_FLAGS= -fPIC -std=c++1y -c $< -o $@ -I/usr/include/python2.7
	LINK_FLAGS= -fPIC -shared -o $(PY_EXT)
	PY_EXT=_maf.so
endif

# for standard compiler settings
SRC_DIR=maf

rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

SRC = $(call rwildcard, $(SRC_DIR), *.cpp) $(SRC_DIR)/maf_wrap.cpp
HEADERS = $(filter-out %maf_wrap.hpp, $(call rwildcard, $(SRC_DIR), *.hpp))
OBJ = $(patsubst %.cpp,$(BUILD_DIR)/%.obj,$(SRC))

ifeq ($(MAKECMDGOALS),debug)
BUILD_DIR=debug
COMPILER_FLAGS+= /debug:all /check:all /check:noarg_temp_created
LINK_FLAGS+= /debug
else
COMPILER_FLAGS+= -O2
BUILD_DIR=release
endif


.PHONY: version.cpp test

all: release test

release: $(PY_EXT)

debug: $(PY_EXT)

clean:
	-rm -rf ./debug/ ./release/ $(SRC_DIR)/maf_wrap.cpp $(SRC_DIR)/maf_wrap.hpp

tags: $(SRC)
	ctags $(SRC)

$(exe): $(OBJ) 
	$(LD) $(OBJ) -o $(exe)  $(LDFLAGS)

version.cpp:
	@echo '#include "version.hpp"' > version.cpp
	@echo '' >> version.cpp
	@echo 'std::string Version::git_sha1 = "`git rev-parse HEAD`"' >> version.cpp
	@echo '' >> version.cpp
	@echo 'std::string Version::git_description = "`git describe --always --dirty`"' >> version.cpp
	@echo "" >> version.cpp

test: $(PY_EXT)
	PYTHONPATH=. mpiexec -n 2 python tests/test.py

$(SRC_DIR)/maf_wrap.cpp $(SRC_DIR)/maf_wrap.hpp: $(HEADERS) maf.i
	swig -python -builtin -includeall -ignoremissing -c++ -outdir . -o $(SRC_DIR)/maf_wrap.cpp -oh $(SRC_DIR)/maf_wrap.hpp maf.i

$(BUILD_DIR)/%.obj: %.cpp %.hpp
	@-if [ ! -d "$(@D)" ]; then python -c 'import os; os.makedirs("$(@D)")' ; fi
	$(COMPILER) $(COMPILER_FLAGS) -I./$(BUILD_DIR)/ -I.

$(PY_EXT): $(OBJ)
	$(LINKER) $(OBJ) $(LINK_FLAGS) 
