CC = gcc
FLAGS = -g 
LIBFLAG = -shared 

CALC = $(wildcard ./*.c ./connect_library/*.c ./menu_work/*.c ./parse/*.c)
CALCOBJ = $(CALC:.c=.o)

%.o: %.c
	$(CC) $(FLAGS) -ldl -c $<

all: $(CALCOBJ)
	$(CC) $(FLAGS) -ldl -o AppCalc *.o