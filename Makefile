all: hilos ejer1

hilos: hilos.c
	gcc -o hilos hilos.c -lm -lpthread
ejer1: ejer1.c
	gcc -o ejer1 ejer1.c -lm
clean:
	rm hilos ejer1