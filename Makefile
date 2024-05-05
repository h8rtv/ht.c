CC = gcc
CFLAGS = -Wall -Wextra -g
SRCDIR = src
INCDIR = include
BINDIR = bin

# List of source files
SRCS = $(wildcard $(SRCDIR)/*.c)

# List of object files
OBJS = $(SRCS:.c=.o)

# Name of the executable
TARGET = hashtable_test

# Build rule
all: $(BINDIR)/$(TARGET)

# Compile source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INCDIR)

$(BINDIR)/$(TARGET): $(OBJS) test/test.o | $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@

$(BINDIR):
	mkdir -p $(BINDIR)

# Clean rule
clean:
	rm -f $(BINDIR)/* $(SRCDIR)/*.o test/*.o
	rm -rf bin/

