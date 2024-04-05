CC = gcc
FLAGS = -g
LIBFLAG = -shared

CALC = $(wildcard ./*.c ./connect_library/*.c ./menu_work/*.c ./parse/*.c)
CALCOBJ = $(CALC:.c=.o)

%.o: %.c
	$(CC) $(FLAGS) -c -g $<

all: $(CALCOBJ)
	$(CC) $(FLAGS) -o -ldl AppCalc *.o

clean:
	rm -f AppCalc *.o
