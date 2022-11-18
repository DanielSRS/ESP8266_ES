run: all
		sudo ./Rasp -analogic -d."D0".16 -d."D1".5

all: Rasp

Rasp: Rasp.o
		gcc main.o -o Rasp

Rasp.o: main.c
		gcc -c main.c -o main.o

clean:
		rm -rf *.o *~ Rasp