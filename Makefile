CC = gcc

BINDIR = ./bin
INCLUDE = ./include
LIBDIR = ./lib
SRCDIR = ./src

CFLAGS = -Wall 
LDFLAGS = -lm
# LDFLAGS = -L$(LIBDIR)
# LDLIBS = -linstrumental

SRC = $(SRCDIR)/main.c 
OBJS = $(SRCDIR)/main.o $(SRCDIR)/instrumental.o
# LIB = $(LIBDIR)/libinstrumental.a

TARGET = $(BINDIR)/main

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)


.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJ)