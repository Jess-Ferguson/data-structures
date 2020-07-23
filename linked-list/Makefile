CC := gcc
SRCDIR := src
DEPDIR := include
CFLAGS := -Wall -Wextra -Wpedantic

LIBS := -lpthread

_DEPS := linked-list.h
DEPS := $(patsubst %,$(DEPDIR)/%,$(_DEPS))

linked-list.o: $(SRCDIR)/linked-list.c
	$(CC) -c $? $(INCLUDE) $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o