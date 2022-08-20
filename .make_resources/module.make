include module.cfg

# Enable parallelism
ifneq ($(AS_DEP),1)
MAKEFLAGS := --jobs=$(shell nproc)
endif

PROJECT_ROOT=..
EXTLIB=$(PROJECT_ROOT)/extlib
EXTLIB_BIN=$(EXTLIB)/bin
EXTLIB_LIB=$(EXTLIB)/lib
EXTLIB_INCLUDE=$(EXTLIB)/include

DISTDIR=$(PROJECT_ROOT)/dist
DISTDIR_BIN=$(DISTDIR)/bin
DISTDIR_LIB=$(DISTDIR)/lib
DISTDIR_INCLUDE=$(DISTDIR)/include

include $(PROJECT_ROOT)/project.cfg

# USEFUL VARIABLES AND FUNCTIONS

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
ifeq ($(MODE), release)
	BUILD=$(BUILDDIR)/release
else
	BUILD=$(BUILDDIR)/debug
endif
MODULE_BUILD=$(BUILD)/$(MODULE_NAME)
BINDIR=$(BUILD)/bin
OBJDIR=$(BUILD)/obj/$(MODULE_NAME)

# OS dependent stuff
ifeq ($(OS),Windows_NT)
	LIB_EXT=.dll
	EXE_EXT=.exe
    OS_DEFINE=-DWINDOWS
	LD_PATH=PATH=$$PATH:$(BINDIR):$(EXTLIB_BIN)
else
	LIB_EXT=.so
    OS_DEFINE=-DLINUX
	LD_PATH=LD_LIBRARY_PATH=$(BINDIR):$(EXTLIB_BIN)
endif

# Output file
OUTPUT=$(PROJECT_NAME)-$(MODULE_NAME)
ifeq ($(TYPE),lib)
	OUTPUT:=lib$(OUTPUT)$(LIB_EXT)
else
	OUTPUT:=$(OUTPUT)$(EXE_EXT)
endif
OUTPUT:=$(BINDIR)/$(OUTPUT)

# .so deps
USEMOD_OUTPUTS=$(USEMOD:%=$(BINDIR)/lib$(PROJECT_NAME)-%$(LIB_EXT))

# Include variables
INCLUDEDIR=include                   # Get dependencies include path
IFLAGS=-Iinclude -I$(EXTLIB_INCLUDE) $(USEMOD:%=-I$(PROJECT_ROOT)/%/include)

# Linking flags                                     # Get dependencies link flag
LFLAGS=-L$(BINDIR) -L$(EXTLIB_BIN) -L$(EXTLIB_LIB) $(USEMOD:%=-l$(PROJECT_NAME)-%)

LFLAGS+=$(LINKLIB:%=-l%)
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

# https://github.com/google/googletest/issues/2686
ifeq ($(OS),Windows_NT)
TEST_GTEST_OUTPUT_FILE:=//$(TEST_GTEST_OUTPUT_FILE)
endif

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
TEST_DEPS=$(TEST_OBJ:.o=.d)

OTHER=$(OS_DEFINE)
ifeq ($(MODE), release)
	CFLAGS+=-O3
	LFLAGS+=-s
else
	CFLAGS+=-g -Og
	OTHER+=-DDEBUG
endif

ifeq ($(TYPE),lib)
	OTHER+=-fPIC
endif

all: $(OUTPUT)

ifeq ($(TYPE),lib)
ifdef T
FILTER=GTEST_FILTER=*$(T)*
endif
test: $(TEST_OUTPUT)
	@echo "Running tests for module $(MODULE_NAME)"
	@mkdir -p $(TEST_GTEST_OUTPUT_DIR)
	$(LD_PATH) TEST_OUTPUTS=$(TEST_OUTPUTS) GTEST_OUTPUT=xml:$(TEST_GTEST_OUTPUT_FILE) $(FILTER) ./$(TEST_OUTPUT)
valgrindtest: $(TEST_OUTPUT)
	@echo "Running tests for module $(MODULE_NAME) through valgrind"
	@mkdir -p $(TEST_GTEST_OUTPUT_DIR)
	$(LD_PATH) TEST_OUTPUTS=$(TEST_OUTPUTS) GTEST_OUTPUT="xml:$(TEST_GTEST_OUTPUT_FILE)" $(FILTER) valgrind $(VALGRIND_OPT) $(TEST_OUTPUT)

else
test:
	@echo "Cannot test an executable"

valgrindtest:
	@echo "Cannot test an executable"
endif

run: $(OUTPUT)
ifeq ($(TYPE),exe)
	@echo "Running $(OUTPUT)"
	@$(LD_PATH) ./$(OUTPUT) $(ARGS)
else
	$(error "Cannot run a library")
endif

gdb: $(OUTPUT)
ifeq ($(TYPE),exe)
	@echo "Running $(OUTPUT)"
	@$(LD_PATH) gdb ./$(OUTPUT) $(ARGS)
else
	$(error "Cannot run a library")
endif

valgrind: $(OUTPUT)
ifeq ($(TYPE),exe)
	@echo "Running $(OUTPUT)"
	@$(LD_PATH) valgrind $(VALGRIND_OPT) ./$(OUTPUT) $(ARGS)
else
	$(error "Cannot run a library")
endif

$(USEMOD_OUTPUTS): $(USEMOD)

$(USEMOD):
	@$(MAKE) AS_DEP=1 --directory=$(PROJECT_ROOT)/$(filter $(USEMOD),$@) $(filter-out valgrindtest test run gdb valgrind,$(MAKECMDGOALS))

dist: $(OUTPUT)
	@mkdir -p $(DISTDIR)
	@echo Copying $(OUTPUT) to $(DISTDIR)/bin...
	@cp $(OUTPUT) $(DISTDIR)
	@echo Copying extlibs to $(DISTDIR)...
	@if [ -d "$(EXTLIB_BIN)" ]; then cp $(EXTLIB_BIN)/* $(DISTDIR_BIN); fi
	@if [ -d "$(EXTLIB_LIB)" ]; then cp $(EXTLIB_LIB)/* $(DISTDIR_LIB); fi
	@if [ -d "$(EXTLIB_INCLUDE)" ]; then cp $(EXTLIB_INCLUDE)/* $(DISTDIR_INCLUDE); fi
	#@echo Compressing dist directory...
	#@cd $(MODULE_BUILD) && zip -r $(NAME).zip $(NAME) > /dev/null

$(OUTPUT): $(USEMOD_OUTPUTS) $(OBJ)
# $@ : Target name
	@echo "Linking $@"
	@mkdir -p $(BINDIR)
	@$(LD_PATH) $(COMPILER) -o $(OUTPUT) $(OBJ) $(LFLAGS) $(OTHER)

$(OBJDIR)/%.o: $(SRCDIR)/%$(SRC_EXT)
	@# $< : First dependency
	@echo "Compiling $<"
	@mkdir -p $(OBJDIR)
	@$(COMPILER) $(CFLAGS) $(IFLAGS) -o $@ -c $< $(OTHER)

$(TEST_OUTPUT): $(USEMOD_OUTPUTS) $(OUTPUT) $(TEST_OBJ)
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
-include $(TEST_DEPS)

clean:
	@rm -r $(BUILDDIR)

distclean:
	@rm -r $(DISTDIR)

.PHONY: all dist run gdb test clean distclean valgrind valgrindtest newclass
