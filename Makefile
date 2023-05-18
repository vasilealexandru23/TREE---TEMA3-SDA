# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -Wshadow -Wpedantic -std=c99 -O0 -g

# define targets
TARGETS=kNN mk

#define object-files
OBJ=mk.o kNN.o

build: 
	$(CC) $(CFLAGS) *.c *.h -o mk

pack:
	zip -FSr 314CA_AlexandruGabrielVasile_Tema3.zip README.md Makefile *.c *.h

clean:
	rm -f $(TARGETS) $(OBJ)

.PHONY: pack clean
