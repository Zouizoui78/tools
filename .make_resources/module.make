include module.cfg

PROJECT_ROOT=..
EXTLIB=$(PROJECT_ROOT)/extlib
include $(PROJECT_ROOT)/project.cfg

# USEFUL VARIABLES AND FUNCTIONS

LD_PATH=LD_LIBRARY_PATH=$(BINDIR):$(EXTLIB)/bin

INCLUDE_PATH=include
HEADER_EXT=.hpp
SRC_EXT=.cpp

upper = $(shell echo "$(1)" | tr a-z A-Z)
lower = $(shell echo "$(1)" | tr A-Z a-z)

VALGRIND_OPT = --leak-check=full --show-leak-kinds=all --trace-children=no --track-origins=yes --suppressions=$(PROJECT_ROOT)/.make_resources/valgrind.supp -s


#===========================================================
#===========================================================

# MODULE MANAGEMENT TARGETS

# Create a new pre-formatted class.
# Copy files from .make/resources/newclass in the module
# and sed some variables with sed.
ifeq ($(word 1, $(MAKECMDGOALS)),newclass)
CLASS_NAME=$(word 2, $(MAKECMDGOALS))
CLASS_NAME_UPPER=$(call upper,$(CLASS_NAME))
HEADERS_PATH=$(INCLUDE_PATH)/$(PROJECT_NAME)/$(MODULE_NAME)
HEADER=$(PROJECT_NAME)/$(MODULE_NAME)/$(CLASS_NAME).hpp
HEADER_PATH=$(INCLUDE_PATH)/$(HEADER)
SOURCE=src/$(CLASS_NAME).cpp
TEST=test/Test$(CLASS_NAME).cpp
newclass:
ifeq ($(CLASS_NAME),)
	@echo "syntax : make newclass <class name>"
else
	@echo "Creating class $(CLASS_NAME)"
	mkdir -p $(HEADERS_PATH)
	cp $(PROJECT_ROOT)/.make_resources/newclass/header.hpp $(HEADER_PATH)
	cp $(PROJECT_ROOT)/.make_resources/newclass/src.cpp $(SOURCE)
	cp $(PROJECT_ROOT)/.make_resources/newclass/test.cpp $(TEST)
	sed -i 's CLASS_NAME_UPPER $(CLASS_NAME_UPPER) g' $(HEADER_PATH)
	sed -i 's HEADER $(HEADER) g' $(SOURCE)
	sed -i 's HEADER $(HEADER) g' $(TEST)
	sed -i 's CLASS_NAME $(CLASS_NAME) g' $(HEADER_PATH)
	sed -i 's CLASS_NAME $(CLASS_NAME) g' $(SOURCE)
	sed -i 's CLASS_NAME $(CLASS_NAME) g' $(TEST)
	sed -i 's PROJECT_NAME $(PROJECT_NAME) g' $(HEADER_PATH)
	sed -i 's PROJECT_NAME $(PROJECT_NAME) g' $(SOURCE)
	sed -i 's PROJECT_NAME $(PROJECT_NAME) g' $(TEST)
	sed -i 's MODULE_NAME $(MODULE_NAME) g' $(HEADER_PATH)
	sed -i 's MODULE_NAME $(MODULE_NAME) g' $(SOURCE)
	sed -i 's MODULE_NAME $(MODULE_NAME) g' $(TEST)
endif

$(CLASS_NAME):
	@:

endif


#===========================================================
#===========================================================

# BUILDING TARGETS

COMPILER=g++

# -MMD -MP generates .d files that are Makefiles
# containing header dependencies for each object file.
CFLAGS=-std=c++$(CPP) -Wall -Wextra -Werror -MMD -MP

# Output dirs
BUILDDIR=$(PROJECT_ROOT)/build
ifeq ($(word 1, $(MAKECMDGOALS)), release)
	BUILD=$(BUILDDIR)/release
else ifeq ($(PLATFORM),windows)
	BUILD=$(BUILDDIR)/windows
else
	BUILD=$(BUILDDIR)/debug
endif
MODULE_BUILD=$(BUILD)/$(MODULE_NAME)
BINDIR=$(BUILD)/bin
OBJDIR=$(BUILD)/obj/$(MODULE_NAME)

# OS dependent stuff
ifeq ($(OS),Windows_NT)
	LIB_EXT=.lib
	EXE_EXT=.exe
    OS_DEFINE=-D WINDOWS
else ifeq ($(PLATFORM),windows)
	LIB_EXT=.lib
	EXE_EXT=.exe
    OS_DEFINE=-D WINDOWS
else
	LIB_EXT=.so
    OS_DEFINE=-D LINUX
endif

# Output file
OUTPUT=$(PROJECT_NAME)-$(MODULE_NAME)
ifeq ($(TYPE),lib)
	OUTPUT:=lib$(OUTPUT)$(LIB_EXT)
else
	OUTPUT:=$(OUTPUT)$(EXE_EXT)
endif
OUTPUT:=$(BINDIR)/$(OUTPUT)

# Include variables
INCLUDEDIR=include                   # Get dependencies include path
IFLAGS=-Iinclude -I$(EXTLIB)/include $(USEMOD:%=-I$(PROJECT_ROOT)/%/include)

# Linking flags                                     # Get dependencies link flag
LFLAGS=-L$(BINDIR) -L$(EXTLIB)/bin -L$(EXTLIB)/lib $(USEMOD:%=-l$(PROJECT_NAME)-%)

# Often times Windows dlls have different name than their Linux equivalent
# e.g. libcurl.so -> libcurl-x64.dll
# Renaming the Windows dll doesn't help because the name is "hardcoded" in the dll
# so even if we link to libcurl.dll the exe will look for libcurl-x64.dll at runtime.
# Additionally when targeting Windows we should statically link the standard libs.
# For these reasons we need separate linking logic and variables for Windows and Linux.
MINGW_LFLAGS=$(WINDOWS_LINKLIB:%=-l%) -static-libgcc -static-libstdc++
ifeq ($(PLATFORM),windows)
	LFLAGS+=$(MINGW_LFLAGS)
else ifeq ($(OS),Windows_NT)
	LFLAGS+=$(MINGW_LFLAGS)
else
    LFLAGS+=$(LINKLIB:%=-l%)
endif
ifeq ($(TYPE),lib)
	LFLAGS+=-shared
endif

# SRCs variables
SRCDIR=src
# List all files in module's src directory
SRC=$(shell ls $(SRCDIR) | grep $(SRC_EXT))

# Tests variables
# A bit ugly to redefine everything for tests but it's easier that way
TESTDIR=$(BUILDDIR)/test
TEST_OUTPUTS=$(CURDIR)/$(TESTDIR)/output/$(MODULE_NAME)
TEST_GTEST_OUTPUT_DIR=$(TESTDIR)/gtest_reports
TEST_GTEST_OUTPUT_FILE=$(TEST_GTEST_OUTPUT_DIR)/$(MODULE_NAME).xml
TEST_BINDIR=$(TESTDIR)/bin
TEST_OBJDIR=$(TESTDIR)/obj/$(MODULE_NAME)
TEST_SRCDIR=test
TEST_SRC=$(shell ls $(TEST_SRCDIR) | grep $(SRC_EXT))
TEST_OBJ=$(TEST_SRC:%$(SRC_EXT)=$(TEST_OBJDIR)/%.o)
TEST_OUTPUT=$(TEST_BINDIR)/test_$(MODULE_NAME)$(EXE_EXT)
TEST_LFLAGS=$(filter-out -shared,$(LFLAGS)) -l:$(notdir $(OUTPUT))
TEST_LFLAGS+=-lgtest -lpthread

# String substitution : *.cpp => $(OBJDIR)/*.o
OBJ=$(SRC:%$(SRC_EXT)=$(OBJDIR)/%.o)

# String substitution : $(OBJDIR)/*.o => $(OBJDIR)/*.d
DEPS=$(OBJ:.o=.d)

OTHER=$(OS_DEFINE)
ifeq ($(word 1, $(MAKECMDGOALS)), release)
	OTHER+=-O2 -s
else ifeq ($(PLATFORM),windows)
# If cross-compiling
	OTHER+=-O2 -s
	COMPILER=x86_64-w64-mingw32-g++
else
	OTHER+=-g -D DEBUG
endif

ifeq ($(TYPE),lib)
	OTHER+=-fPIC
endif

all: $(USEMOD) $(OUTPUT)
release: $(USEMOD) $(OUTPUT)

ifeq ($(TYPE),lib)
ifdef T
FILTER=GTEST_FILTER=*$(T)*
endif
test: $(USEMOD) $(OUTPUT) $(TEST_OUTPUT)
	@echo "Running tests for module $(MODULE_NAME)"
	@mkdir -p $(TEST_GTEST_OUTPUT_DIR)
ifeq ($(OS),Windows_NT)
	PATH=$(BINDIR) TEST_OUTPUTS=$(TEST_OUTPUTS) GTEST_OUTPUT="xml:$(TEST_GTEST_OUTPUT_FILE)" $(FILTER) ./$(TEST_OUTPUT)
else
	$(LD_PATH) TEST_OUTPUTS=$(TEST_OUTPUTS) GTEST_OUTPUT="xml:$(TEST_GTEST_OUTPUT_FILE)" $(FILTER) ./$(TEST_OUTPUT)
endif
valgrindtest: $(USEMOD) $(OUTPUT) $(TEST_OUTPUT)
	@echo "Running tests for module $(MODULE_NAME) through valgrind"
	@mkdir -p $(TEST_GTEST_OUTPUT_DIR)
	$(LD_PATH) TEST_OUTPUTS=$(TEST_OUTPUTS) GTEST_OUTPUT="xml:$(TEST_GTEST_OUTPUT_FILE)" $(FILTER) valgrind $(VALGRIND_OPT) $(TEST_OUTPUT)

else
test:
	@echo "Cannot test an executable"

valgrindtest:
	@echo "Cannot test an executable"
endif

run: $(USEMOD) $(OUTPUT)
ifeq ($(TYPE),exe)
	@echo "Running $(OUTPUT)"
	@$(LD_PATH) ./$(OUTPUT)
else
	$(error "Cannot run a library")
endif

gdb: $(USEMOD) $(OUTPUT)
ifeq ($(TYPE),exe)
	@echo "Running $(OUTPUT)"
	@$(LD_PATH) gdb ./$(OUTPUT)
else
	$(error "Cannot run a library")
endif

valgrind: $(USEMOD) $(OUTPUT)
ifeq ($(TYPE),exe)
	@echo "Running $(OUTPUT)"
	@$(LD_PATH) valgrind $(VALGRIND_OPT) ./$(OUTPUT)
else
	$(error "Cannot run a library")
endif

$(USEMOD):
	@$(MAKE) --directory=$(PROJECT_ROOT)/$@ $(filter-out valgrindtest test run gdb valgrind,$(MAKECMDGOALS))

# TODO : Cross-compil' packaging target. Unused for now.
$(DIST): DISTDIR=$(MODULE_BUILD)/$(NAME)
$(DIST): $(OUTPUT)
	@mkdir -p $(DISTDIR)
	@echo Preparing dist directory...
	@cp $(OUTPUT) $(DISTDIR)/
	@cp -r res $(DISTDIR)/
	@cp extlib/bin/* $(DISTDIR)
	@echo Compressing dist directory...
	@cd $(MODULE_BUILD) && zip -r $(NAME).zip $(NAME) > /dev/null

$(OUTPUT): $(OBJ)
# $@ : Target name
	@echo "Linking $@"
	@mkdir -p $(BINDIR)
	@$(LD_PATH) $(COMPILER) -o $(OUTPUT) $(OBJ) $(LFLAGS) $(OTHER)

$(OBJDIR)/%.o: $(SRCDIR)/%$(SRC_EXT)
	@# $< : First dependency
	@echo "Compiling $<"
	@mkdir -p $(OBJDIR)
	@$(COMPILER) $(CFLAGS) $(IFLAGS) -o $@ -c $< $(OTHER)

$(TEST_OUTPUT): $(TEST_OBJ)
	@echo "Linking $@"
	@mkdir -p $(TEST_BINDIR)
	@$(LD_PATH) $(COMPILER) -o $(TEST_OUTPUT) $(TEST_OBJ) $(TEST_LFLAGS) $(OTHER)

$(TEST_OBJDIR)/%.o: $(TEST_SRCDIR)/%$(SRC_EXT)
	@echo "Compiling $<"
	@mkdir -p $(TEST_OBJDIR)
	@$(COMPILER) $(CFLAGS) $(IFLAGS) -o $@ -c $< $(OTHER)

# Include rules generated by -MMD -MP.
# Since this rules tell how to build object files,
# their dependencies are added to the previous rule.
-include $(DEPS)

clean:
	@rm -r $(BUILDDIR)

.PHONY: all release run gdb test clean valgrind valgrindtest newclass $(USEMOD)
