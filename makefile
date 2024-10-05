ROOT = $(shell pwd)
# Normalise path if on windows
ifeq ($(OS),Windows_NT)
	ROOT := $(shell cygpath -m $(ROOT))
endif

CC = gcc

ifeq ($(NOWERROR),true)
	CFLAGS += -Wall -Wextra -std=c99
else
	CFLAGS += -Wall -Werror -Wextra -std=c99
endif

EXT_LIBS = -lpthread

BUILD_DIR = build
BIN = bin
LIBMEM = libmem
LIBMEM_DIR = $(ROOT)/lib/$(LIBMEM)
SUBPROJECTS = bestfit firstfit

.PHONY: all test clean

# Main target to build everything
all: $(LIBMEM_DIR)/$(BIN)/$(LIBMEM).so $(patsubst %,link-%,$(SUBPROJECTS))

# Build the libmem.so library and link it to the build directory
$(LIBMEM_DIR)/$(BIN)/$(LIBMEM).so:
	@mkdir -p $(BIN)
	@$(MAKE) -C $(LIBMEM_DIR) CC="$(CC)" CFLAGS="$(CFLAGS)" EXT_LIBS="$(EXT_LIBS)"

# Build and link each subproject, passing the library and include paths
link-%: $(LIBMEM_DIR)/$(BIN)/$(LIBMEM).so
	@$(MAKE) -C subprojects/$* CC="$(CC)" CFLAGS="$(CFLAGS)" LDFLAGS="-I$(LIBMEM_DIR)/src -L$(LIBMEM_DIR)/$(BIN) -Wl,-rpath,$(LIBMEM_DIR)/$(BIN) -lmem" EXT_LIBS="$(EXT_LIBS)"
	@mkdir -p $(BIN)
	@ln -rsf subprojects/$*/$(BIN)/$* $(BIN)/$*

# Clean everything
clean: $(patsubst %,clean-%,$(SUBPROJECTS)) clean-$(LIBMEM)
	rm -rf $(BIN)/* $(BUILD_DIR)/*

# Clean the subprojects
clean-%:
	@$(MAKE) -C subprojects/$* clean

# Clean the libmem library
clean-$(LIBMEM):
	@$(MAKE) -C $(LIBMEM_DIR) clean

