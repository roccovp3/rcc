# Makefile

# Compiler
CC := gcc

# Compiler flags
CFLAGS := -Wall -Wextra -g

# Source files
SRCS := $(wildcard src/*.c)

# Object files
OBJS := $(SRCS:.c=.o)

# Executable name
TARGET := rcc

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to build object files from source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)

# PHONY targets
.PHONY: all clean
