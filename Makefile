# update FC & LD for your system
COMPILER=cl
LINKER=link

# for standard compiler settings
SRC_DIR=maf
COMPILER_FLAGS= /nologo /fp:strict /MP "/IC:\Program Files\Microsoft HPC Pack 2012\Inc" "/IC:\Python27\include" /I.
LINK_FLAGS= /nologo /incremental:no '/libpath:C:\Python27\libs' '/libpath:C:\Program Files\Microsoft HPC Pack 2012\Lib\amd64' msmpi.lib

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

PY_EXT=_maf.pyd

.PHONY: dirs version.cpp test

all: release test

release: dirs $(PY_EXT)

debug: dirs $(PY_EXT)

dirs:
	@if [ ! -d "debug" ]; then mkdir "debug" ; fi
	@if [ ! -d "debug\\maf" ]; then mkdir "debug\\maf" ; fi
	@if [ ! -d "release" ]; then mkdir "release" ; fi
	@if [ ! -d "release\\maf" ]; then mkdir "release\\maf" ; fi
	@if [ ! -d "release" ]; then mkdir "release" ; fi

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
	mpiexec -n 2 python tests/test.py

$(SRC_DIR)/maf_wrap.cpp $(SRC_DIR)/maf_wrap.hpp: $(HEADERS) maf.i
	swig -python -builtin -includeall -ignoremissing -c++ -outdir . -o $(SRC_DIR)/maf_wrap.cpp -oh $(SRC_DIR)/maf_wrap.hpp maf.i

$(BUILD_DIR)/%.obj: %.cpp
	@if [ ! -d $(@D) ] ; then mkdir $(@D) ; fi
	$(COMPILER) $(COMPILER_FLAGS) -c $< /Fo:$@ -I./$(BUILD_DIR)/

$(PY_EXT): $(OBJ)
	$(LINKER) $(OBJ) /DLL /out:$(PY_EXT) $(LINK_FLAGS) 
