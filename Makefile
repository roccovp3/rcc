# Compiler
CC := gcc

# Compiler flags
CFLAGS := -Wall -Wextra -g

# Source files
SRCS := $(wildcard src/*.c)

# Object files directory
BUILD_DIR := build
OBJS := $(patsubst src/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Executable name
TARGET := $(BUILD_DIR)/rcc

# Default target
all: $(TARGET)

# Rule to create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Rule to build the executable
$(TARGET): $(BUILD_DIR) $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Rule to build object files from source files
$(BUILD_DIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# PHONY targets
.PHONY: all clean
