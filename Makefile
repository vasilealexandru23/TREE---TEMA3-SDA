# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -Wshadow -Wpedantic -std=c99 -O0 -g

# define targets
TARGETS=kNN mk

#define object-files
OBJ=mk.o kNN.o

build: $(TARGETS)

mk: mk.c
	$(CC) $(CFLAGS) mk.c Trie.c Trie.h -o mk

kNN: kNN.c
	$(CC) $(CFLAGS) kNN.c k-dtree.c k-dtree.h -o kNN

pack:
	zip -FSr 314CA_AlexandruGabrielVasile_Tema3.zip README.md Makefile *.c *.h

clean:
	rm -f $(TARGETS) $(OBJ)

.PHONY: pack clean
