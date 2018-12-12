# Sample Makefile

CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra

# Links together files needed to create executable
ac: autocomplete.o
	$(CC) -o ac autocomplete.o

# Compiles mainFile.c to create mainFile.o
# Note that we list functions.h here as a file that
# mainFile.c depends on, since mainFile.c #includes it
autocomplete.o: autocomplete.c
	$(CC) $(CFLAGS) -c autocomplete.c

# Removes all object files and the executable named main,
# so we can start fresh
clean:
	rm -f *.o ac
