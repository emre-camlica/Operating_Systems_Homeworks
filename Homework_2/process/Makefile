CC = gcc
CFLAGS = -Wall

all: Homework2_process

Homework2_process: Homework2_process.o
	$(CC) $(CFLAGS) -o Homework2_process Homework2_process.o

Homework2_process.o: Homework2_process.c
	$(CC) $(CFLAGS) -c Homework2_process.c

clean:
	rm -f Homework2_process.o Homework2_process