
ifeq ($(OS),Windows_NT)
	CXX=cl
	LD=link
	CXXFLAGS= /nologo /EHsc /fp:strict /MP "/IC:\Program Files\Microsoft HPC Pack 2012\Inc" "/IC:\Python27\include" /I. /c $< /Fo:$@
	LDFLAGS= /nologo /incremental:no '/libpath:C:\Python27\libs' '/libpath:C:\Program Files\Microsoft HPC Pack 2012\Lib\amd64' msmpi.lib
	LDSHARED= /DLL /out:$@
	LDEXE= /out:$@
	PY_EXT=_maf.pyd
else
	CXX=mpic++
	LD=mpic++
	CXXFLAGS= -fPIC -std=c++1y -c $< -o $@ -I/usr/include/python2.7
	LDFLAGS= -fPIC
	LDSHARED= -shared -o $@
	LDEXE= -o $@
	PY_EXT=_maf.so
endif

ifeq ($(MAKECMDGOALS),debug)
	BUILD_DIR=debug
	CXXFLAGS+= /Zi
	LDFLAGS+= /debug /LDd
else
	CXXFLAGS+= -O2
	BUILD_DIR=release
endif

# for standard CXX settings
SRC_DIR=maf

# functions
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))
define colorecho
	@tput setaf 4
	@echo $1
	@tput sgr0
	@$1
endef

SRC = $(call rwildcard, $(SRC_DIR), *.cpp) $(SRC_DIR)/maf_wrap.cpp $(SRC_DIR)/Version.cpp
HEADERS = $(filter-out %maf_wrap.hpp maf/maf.hpp, $(call rwildcard, $(SRC_DIR), *.hpp))
DIRECTORIES = $(sort $(dir $(SRC) $(HEADERS)))
OBJ = $(patsubst %.cpp,$(BUILD_DIR)/%.obj,$(SRC))

.PHONY: test

all: release

release: test

debug: test

clean:
	-$(RM) -rf ./debug/ ./release/
	-$(RM) $(SRC_DIR)/maf_wrap.cpp $(SRC_DIR)/maf_wrap.hpp _maf.* maf/Version.cpp tests/test_cpp.exe

tags: $(SRC)
	ctags $(SRC)

maf/maf.hpp: $(HEADERS)
	@echo "// this file is auto-generated, do not modify" > $@
	@$(foreach hpp,$^,echo '#include "$(hpp)"' >> $@;)
	@echo "" >> $@

maf/Version.cpp: $(filter-out %wrap.hpp %wrap.cpp %Version.cpp,$(HEADERS) $(SRC))
	@echo '#include "maf/Version.hpp"' > $@
	@echo '' >> $@
	@echo 'namespace maf {' >> $@
	@echo 'const std::string Version::git_sha1 = "'`git rev-parse HEAD`'";' >> $@
	@echo '' >> $@
	@echo 'const std::string Version::git_description = "'`git describe --always --dirty`'";' >> $@
	@echo '}' >> $@
	@echo "" >> $@

test: test_cpp test_py

test_py: PYTESTFILES=$(call rwildcard, tests/, *.py)
test_py: $(PY_EXT)
	$(foreach pytestfile, $(PYTESTFILES), $(call colorecho,PYTHONPATH=. mpiexec -n 2 python $(pytestfile)))

test_cpp: tests/test_cpp.exe
	$(call colorecho,mpiexec -n 4 $<)

tests/test_cpp.exe: maf/maf.hpp $(patsubst %.cpp,$(BUILD_DIR)/%.obj, $(call rwildcard, tests/, *.cpp)) $(OBJ)
	$(call colorecho,$(LD) $(LDFLAGS) $(filter-out %wrap.obj maf/maf.hpp,$^) $(LDEXE))

$(SRC_DIR)/maf_wrap.cpp $(SRC_DIR)/maf_wrap.hpp: $(HEADERS) maf.i maf/maf.hpp
	$(call colorecho,swig -python -builtin -includeall -ignoremissing -c++ -outdir . -o $(SRC_DIR)/maf_wrap.cpp -oh $(SRC_DIR)/maf_wrap.hpp maf.i)

$(BUILD_DIR)/%.obj: %.cpp %.hpp
	@-if [ ! -d "$(@D)" ]; then python -c 'import os; os.makedirs("$(@D)")' ; fi
	$(call colorecho,$(CXX) $(CXXFLAGS) -I.)

$(BUILD_DIR)/%.obj: %.cpp
	@-if [ ! -d "$(@D)" ]; then python -c 'import os; os.makedirs("$(@D)")' ; fi
	$(call colorecho,$(CXX) $(CXXFLAGS) -I.)

$(PY_EXT): $(OBJ)
	$(call colorecho,$(LD) $(LDSHARED) $^ $(LDFLAGS))
