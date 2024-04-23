all: ex1 

ex1: ex1.o
	gcc -pthread ex1.o -o ex1.out

ex1.o: ex1.c
	gcc -pthread -c ex1.c

clean:
	rm -f *.out *.o