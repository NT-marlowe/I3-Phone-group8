CC = gcc

BINDIR = ./bin
INCLUDE = ./include
LIBDIR = ./lib
SRCDIR = ./src

CFLAGS = -Wall -O2 
LDFLAGS = -lm
# LDFLAGS = -L$(LIBDIR)
# LDLIBS = -linstrumental

SRC = $(SRCDIR)/main.c 
OBJS = $(SRCDIR)/main.o $(SRCDIR)/instrumental.o \
			 $(SRCDIR)/communication.o $(SRCDIR)/die.o
# LIB = $(LIBDIR)/libinstrumental.a

TARGET = $(BINDIR)/main

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)


.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS)