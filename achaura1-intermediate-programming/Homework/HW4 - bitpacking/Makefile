# Sample Makefile

CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra

# Links together files needed to create executable
db: fileUtil.o interface.o main.o choices.o
	$(CC) -o db fileUtil.o interface.o main.o choices.o

# Compiles mainFile.c to create mainFile.o
# Note that we list functions.h here as a file that
# mainFile.c depends on, since mainFile.c #includes it
main.o: main.c fileUtil.h interface.h choices.h
	$(CC) $(CFLAGS) -c main.c

# Compiles functions.c to create functions.o
# Note that we list functions.h here as a file that
# functions.c depends on, since functions.c #includes it
fileUtil.o: fileUtil.c fileUtil.h interface.h
	$(CC) $(CFLAGS) -c fileUtil.c

interface.o: interface.c interface.h
	$(CC) $(CFLAGS) -c interface.c

choices.o: choices.c choices.h fileUtil.h interface.h
	$(CC) $(CFLAGS) -c choices.c


# Removes all object files and the executable named main,
# so we can start fresh
clean:
	rm -f *.o db *~
