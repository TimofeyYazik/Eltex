CC = gcc
FLAGS = -g -Wall -Werror -Wextra
LIBFLAG = -shared 

CALC = $(wildcard AppCalc/*.c)
CALCOBJ = $(CALC:.c=.o)

lib: libobj
  $(CC) $(FLAGS) $(LIBFLAG) CalcLibInt/*.o -o libCalc.so

libobj: 
	gcc -c $(LIBFLAG) CalcLibInt/*.c

appcalc: 
	$(CC) $(FLAGS) -ldl -o AppCalc $(CALCOBJ)