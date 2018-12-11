# Akash Chaurasia (achaura1)
# achaura1@jhu.edu
# 601.220 (02)
# Homework 7
# November 8, 2017


CC = g++
DEBUG = -g
CFLAGS = -std=c++11 -pedantic -Wall -Wextra $(DEBUG)


cluster: main.o dataset.o kmeans.o
	$(CC) -o cluster main.o dataset.o kmeans.o

test: test.o dataset.o kmeans.o
	$(CC) -o test test.o dataset.o kmeans.o
	@./test

test.o: test.cpp test.h
	$(CC) $(CFLAGS) -c test.cpp

main.o: main.cpp dataset.cpp
	$(CC) $(CFLAGS) -c main.cpp

dataset.o: dataset.cpp dataset.h
	$(CC) $(CFLAGS) -c dataset.cpp

kmeans.o: kmeans.cpp kmeans.h
	$(CC) $(CFLAGS) -c kmeans.cpp

clean:
	rm -f *.o cluster test *~ *.txt
