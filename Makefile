include project.cfg

# New module target with protection against wrong syntax.
# This targets copies .make_resources/newmod into <module name>.
ifeq ($(word 1, $(MAKECMDGOALS)),newmod)
MODNAME=$(word 2, $(MAKECMDGOALS))
RES=.make_resources
newmod:
ifeq ($(MODNAME),)
	@echo "syntax : make newmod <module name>"
else
	@echo "Creating module $(MODNAME)"
	mkdir -p $(MODNAME)/include $(MODNAME)/src
	cp -r $(RES)/newmod/* $(MODNAME)
	sed -i 's MODULE_NAME= MODULE_NAME=$(MODNAME) g' $(MODNAME)/module.cfg
endif # MODNAME

$(MODNAME):
	@:

else

MODULES=$(dir $(shell find -name 'module.cfg' -not -path "./.make_resources/*"))

# Build all modules.
all: $(MODULES)

# Build all modules with gcc optimizations enabled.
release: $(MODULES)

# Run tests for all modules.
test: $(MODULES)

# Run tests through valgrind.
valgrindtest: $(MODULES)

# Actual module building target.
$(MODULES):
	@$(MAKE) --directory=$@ $(MAKECMDGOALS)

# Remove build directory.
clean:
	@rm -rf build

endif # newmod

.PHONY: all release test valgrindtest clean newmod $(MODULES)