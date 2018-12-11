#Akash Chaurasia & Parth Vora
#achaura1; pvora4
#achaura1@jhu.edu; pvora4@jhu.edu
#CS.220 Midterm project 1
#16/10/2017

CC=gcc
CFLAGS=-std=c99 -pedantic -Wextra -Wall -g 

main: main.o menu.o imageIO.o imagefunctions.o 
	$(CC) -o main main.o menu.o imageIO.o imagefunctions.o -lm

main.o: main.c menu.h imageIO.h imagefunctions.h pixels.h
	$(CC) $(CFLAGS) -c main.c

menu.o: menu.c menu.h pixels.h
	$(CC) $(CFLAGS) -c menu.c

imageIO.o: imageIO.c imageIO.h pixels.h
	$(CC) $(CFLAGS) -c imageIO.c

imagefunctions.o: imagefunctions.c imagefunctions.h pixels.h
	$(CC) $(CFLAGS) -c imagefunctions.c

clean:
	rm -f *.o main
