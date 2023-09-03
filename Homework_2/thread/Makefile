CC=gcc
CFLAGS=-Wall -pthread

all: Homework2_thread

Homework2_thread: Homework2_thread.o
	$(CC) $(CFLAGS) Homework2_thread.o -o Homework2_thread

Homework2_thread.o: Homework2_thread.c
	$(CC) $(CFLAGS) -c Homework2_thread.c

clean:
	rm -f Homework2_thread.o Homework2_thread