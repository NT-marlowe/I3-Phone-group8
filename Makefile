CC = gcc

BINDER = ./bin
INCLUDE = ./include
LIBDIR = ./lib
SRCDIR = ./src

CFLAGS = -Wall 
# LDFLAGS = -L$(LIBDIR)
# LDLIBS = -linstrumental

SRC = $(SRCDIR)/main.c 
OBJS = $(SRCDIR)/main.o $(SRCDIR)/instrumental.o
# LIB = $(LIBDIR)/libinstrumental.a

TARGET = $(BINDER)/main

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -lm


.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJ)