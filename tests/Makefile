CC := gcc
SRCDIR := src
DEPDIR := include
CFLAGS := -Wall -Wextra -Wpedantic -g

LIBS := -lpthread

_DEPS := hash-table.h stack.h linked-list.h
DEPS := $(patsubst %,$(DEPDIR)/%,$(_DEPS))

hash-table-test: $(SRCDIR)/hash-table-test.c hash-table.o
	$(CC) $? $(INCLUDE) $(CFLAGS) $(LIBS) -o $@

linked-list-test: $(SRCDIR)/linked-list-test.c linked-list.o
	$(CC) $? $(INCLUDE) $(CFLAGS) $(LIBS) -o $@

stack-test: $(SRCDIR)/stack-test.c stack.o
	$(CC) $? $(INCLUDE) $(CFLAGS) $(LIBS) -o $@

.PHONY: clean

clean:
	rm -f *.o hash-table-test linked-list-test stack-test