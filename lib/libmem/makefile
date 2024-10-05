PROJECT_NAME = libmem

BIN = bin
OBJDIR = build
SRCS = $(wildcard src/*.c)
HDRS = $(wildcard src/*.h)

# Object files derived from source files
OBJS = $(SRCS:src/%.c=$(OBJDIR)/%.o)

# Output shared library (.so)
TARGET = $(BIN)/$(PROJECT_NAME).so

.PHONY: all clean

all: $(TARGET)

# Create shared library from object files
$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CC) -shared -o $@ $^ $(EXT_LIBS)

# Compile each source file into object files
$(OBJDIR)/%.o: src/%.c $(HDRS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -fPIC -c $< -o $@  # Ensure -fPIC for shared library

# Clean up build artifacts
clean:
	rm -rf $(OBJDIR)/* $(BIN)/*

