all: ex1 ex2 ex3

ex1: ex1.o
	gcc -pthread ex1.o -o ex1.out

ex2: ex2.o
	gcc -pthread ex2.o -o ex2.out

ex3: ex3.o
	gcc -pthread ex3.o -o ex3.out

clean:
	rm -f *.out *.o