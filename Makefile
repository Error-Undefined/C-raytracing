# Define the compiler and the linker.
CXX = gcc
CC  = $(CXX)

# Generate dependencies in *.d files
DEPFLAGS = -MT $@ -MMD -MP -MF $*.d

# Define preprocessor, compiler, and linker flags.
CFLAGS =  -Og -Wall -Wextra -pedantic-errors
CFLAGS += -g
CFLAGS += $(DEPFLAGS)
LDFLAGS = -g


# Targets
PROGS = main
all: $(PROGS)

# Targets rely on implicit rules for compiling and linking
all_tests: main.o

# Phony targets
.PHONY: all test clean

# Standard clean
clean:
	rm -f *.o $(PROGS)

# Clean .d files
depclean:
	rm -f *.d

# Include the *.d files
SRC = $(wildcard *.c)
-include $(SRC:.c=.d)
