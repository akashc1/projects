# Akash Chaurasia (achaura1)
# achaura1@jhu.edu
# 601.220 (02)
# Homework 6
# November 1, 2017


CC = g++
DEBUG = -g
CFLAGS = -std=c++11 -pedantic -Wall -Wextra $(DEBUG)


ac: main.o dbload.o edit.o select.o
	$(CC) -o ac main.o dbload.o edit.o select.o

main.o: main.cpp dbload.cpp edit.cpp select.cpp
	$(CC) $(CFLAGS) -c main.cpp

test: test.o dbload.o edit.o select.o
	$(CC) -o test test.o dbload.o edit.o select.o
	@./test

test.o: test.cpp dbload.cpp edit.cpp select.cpp
	$(CC) $(CFLAGS) -c test.cpp


dbload.o: dbload.cpp dbload.h
	$(CC) $(CFLAGS) -c dbload.cpp

edit.o: edit.cpp edit.h
	$(CC) $(CFLAGS) -c edit.cpp

select.o: select.cpp select.h
	$(CC) $(CFLAGS) -c select.cpp

clean:
	rm -f *.o ac *~ test *.txt
