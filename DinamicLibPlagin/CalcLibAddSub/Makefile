CC = gcc
FLAGS = -g -Wall -Werror -Wextra
LIBFLAG = -shared 

CALC = $(wildcard *.c)
CALCOBJ = $(CALC:.c=.o)

%.o: %.c
	$(CC) $(FLAGS) -c $<

lib: $(CALCOBJ)
	$(CC) $(FLAGS) $(LIBFLAG) *.o -o libcalc1.so
	mv libcalc1.so ../

clean:
	rm -f ../libcalc1.so *.o
