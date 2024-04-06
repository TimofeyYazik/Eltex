CC = gcc
FLAGS = -g -Wall -Werror -Wextra
LIBFLAG = -shared 

CALC = $(wildcard *.c)
CALCOBJ = $(CALC:.c=.o)

%.o: %.c
	$(CC) $(FLAGS) -c $<

lib: $(CALCOBJ)
	$(CC) $(FLAGS) $(LIBFLAG) *.o -o libcalc2.so
	mv libcalc2.so ../

clean:
	rm -f ../libcalc2.so *.o
