CC = gcc
FLAGS = -g -Wall -Werror -Wextra
LIBFLAG = -shared 

CALC = $(wildcard CalcLibInt/*.c)
CALCOBJ = $(CALC:.c=.o)

%.o: %.c
	$(CC) $(FLAGS) -c $<

lib: $(CALCOBJ)
	$(CC) $(FLAGS) $(LIBFLAG) *.o -o libcalc.so

clean:
	rm -f libcalc.so *.o
