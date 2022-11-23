run: all
		sudo ./Rasp -analogic -d."D0".16 -d."D1".5

list: all
		sudo ./Rasp -l

all: Rasp

Rasp: Rasp.o display.o
		gcc main.o display.o -o Rasp

Rasp.o: main.c
		gcc -c main.c -o main.o

display.o: display.s
		gcc -c display.s -o display.o

clean:
		rm -rf *.o *~ Rasp