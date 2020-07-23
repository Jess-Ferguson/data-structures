CC := gcc
SRCDIR := src
DEPDIR := include
CFLAGS := -Wall -Wextra -Wpedantic

LIBS := -lpthread

_DEPS := stack.h
DEPS := $(patsubst %,$(DEPDIR)/%,$(_DEPS))

stack.o: $(SRCDIR)/stack.c
	$(CC) -c $? $(INCLUDE) $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o