CC := gcc
SRCDIR := src
DEPDIR := include
CFLAGS := -Wall -Wextra -Wpedantic -g

LIBS := -lpthread

_DEPS := hash-table.h
DEPS := $(patsubst %,$(DEPDIR)/%,$(_DEPS))

hash-table.o: $(SRCDIR)/hash-table.c
	$(CC) -c $? $(INCLUDE) $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o