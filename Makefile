CC = gcc

BINDIR = ./bin
INCLUDE = ./include
LIBDIR = ./lib
SRCDIR = ./src

CFLAGS = -Wall -O2 -Wno-unused-result
LDFLAGS = -lm -lpthread

# LDFLAGS = -L$(LIBDIR)

SRC = $(SRCDIR)/main.c 

OBJS = $(SRCDIR)/main.o $(SRCDIR)/instrumental.o \
			 $(SRCDIR)/communication.o $(SRCDIR)/die.o \
			 $(SRCDIR)/mypthread.o

TARGET = $(BINDIR)/main

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)


.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS)
