CC = gcc
FLAGS = -g -Wall -Werror -Wextra
LIBFLAG = -shared 

CALC = $(wildcard AppCalc/*.c)
CALCOBJ = $(CALC:.c=.o)

%.o: %.c
	$(CC) $(FLAGS) -ldl -c $<

all: $(CALCOBJ)
	$(CC) $(FLAGS) -ldl -o AppCalc $(CALCOBJ)