include project.cfg

# New module target with protection against wrong syntax.
# This targets copies .make_resources/newmod into src/<module name>.
ifeq ($(word 1, $(MAKECMDGOALS)),newmod)
MODNAME=$(word 2, $(MAKECMDGOALS))
MODPATH=src/$(MODNAME)
RES=.make_resources
newmod:
ifeq ($(MODNAME),)
	@echo "syntax : make newmod <module name>"
else
	@echo "Creating module $(MODNAME)"
	mkdir -p $(MODPATH)/include $(MODPATH)/src
	cp -r $(RES)/newmod/* $(MODPATH)
	sed -i 's MODULE_NAME= MODULE_NAME=$(MODNAME) g' $(MODPATH)/module.cfg
endif # MODNAME

$(MODNAME):
	@:

else

# TODO : Smart dependency graph building ?
MODULES_PATH=src/*

# Build all modules.
all: $(MODULES_PATH)

# Build all modules with gcc optimizations enabled.
release: $(MODULES_PATH)

# Run tests for all modules.
test: $(MODULES_PATH)

# Run tests through valgrind.
valgrindtest: $(MODULES_PATH)

# Actual module building target.
$(MODULES_PATH):
	@$(MAKE) --directory=$@ $(MAKECMDGOALS)

# Remove build directory.
clean:
	@rm -rf build

endif # newmod

.PHONY: all release test valgrindtest clean newmod $(MODULES_PATH)