
ifeq ($(OS),Windows_NT)
	CXX=cl
	CXXFLAGS= /nologo /EHa /fp:strict /MP "/IC:\Program Files\Microsoft HPC Pack 2012\Inc" "/IC:\Python27\include" /I. /c $< /Fo:$@
	LDFLAGS= /nologo /incremental:no '/libpath:C:\Python27\libs' '/libpath:C:\Program Files\Microsoft HPC Pack 2012\Lib\amd64' msmpi.lib
	LDSHARED=link $(LDFLAGS) /DLL /out:$@
	LDSTATIC=lib $(LDFLAGS) /out:$@
	LDEXE=link $(LDFLAGS) /out:$@
	LIBMAF=lib/libmaf.lib
	PY_EXT=_maf.pyd
else
	CXX=mpic++
	LD=mpic++
	CXXFLAGS= -fPIC -Wall -std=c++1y -c $< -o $@ -I/usr/include/python2.7
	LDFLAGS= -fPIC
	LDSHARED=mpic++ -Wall -shared -o $@
	LDSTATIC=ar crs $@
	LDEXE=mpic++ -Wall -o $@
	LIBMAF=lib/libmaf.a
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

SRC = $(filter-out %_wrap%,$(call rwildcard, $(SRC_DIR), *.cpp)) $(SRC_DIR)/Version.cpp
HEADERS = $(filter-out %maf_wrap.hpp maf/maf.hpp, $(call rwildcard, $(SRC_DIR), *.hpp))
DIRECTORIES = $(sort $(dir $(SRC) $(HEADERS)))
OBJ = $(patsubst %.cpp,$(BUILD_DIR)/%.obj,$(SRC))

.PHONY: test

all: release

release: test

debug: test

clean:
	-$(RM) -r ./debug/ ./release/ ./lib/
	-$(RM) maf/maf.hpp maf/Version.cpp
	-$(RM) $(SRC_DIR)/maf_wrap.cpp $(SRC_DIR)/maf_wrap.hpp _maf.* maf.py maf.pyc
	-$(RM) tests/*.exe tests/*.exp tests/*.lib

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

test_py: PY_TEST_FILES=$(sort $(call rwildcard, tests/, *.py))
test_py: $(PY_EXT)
	$(foreach pytestfile, $(PY_TEST_FILES), $(call colorecho,PYTHONPATH=. mpiexec -n 2 python $(pytestfile)))

$(LIBMAF): $(OBJ)
	@-if [ ! -d "$(@D)" ]; then python -c 'import os; os.makedirs("$(@D)")' ; fi
	$(call colorecho,$(LDSTATIC) $^)

$(PY_EXT): $(LIBMAF) $(BUILD_DIR)/maf/maf_wrap.obj
	@# $< is repeated (implicit in $^) because the symbols are only needed after maf_wrap.obj
	$(call colorecho,$(LDSHARED) $^ $<)

test_cpp: $(sort $(patsubst %.cpp, %.exe, $(call rwildcard, tests/, *.cpp)))
	$(foreach cpp_test_exec, $^, $(call colorecho,mpiexec -n 3 $(cpp_test_exec)))

tests/%.exe: maf/maf.hpp $(BUILD_DIR)/tests/%.obj $(LIBMAF)
	$(call colorecho,$(LDEXE) $(filter-out maf/maf.hpp,$^))

$(SRC_DIR)/maf_wrap.cpp $(SRC_DIR)/maf_wrap.hpp: maf.i maf/maf.hpp
	$(call colorecho,swig -python -builtin -includeall -ignoremissing -c++ -outdir . -o $(SRC_DIR)/maf_wrap.cpp -oh $(SRC_DIR)/maf_wrap.hpp maf.i)

$(BUILD_DIR)/%.obj: %.cpp %.hpp
	@-if [ ! -d "$(@D)" ]; then python -c 'import os; os.makedirs("$(@D)")' ; fi
	$(call colorecho,$(CXX) $(CXXFLAGS) -I.)

$(BUILD_DIR)/%.obj: %.cpp
	@-if [ ! -d "$(@D)" ]; then python -c 'import os; os.makedirs("$(@D)")' ; fi
	$(call colorecho,$(CXX) $(CXXFLAGS) -I.)
