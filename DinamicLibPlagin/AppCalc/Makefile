CC = gcc
FLAGS = -g
LIBFLAG = -shared

CALC = $(wildcard ./*.c ./connect_library/*.c ./menu_work/*.c ./parse/*.c)
CALCOBJ = $(CALC:.c=.o)

%.o: %.c
  $(CC) $(FLAGS) -c $<

all: $(CALCOBJ)
  $(CC) $(FLAGS) -o AppCalc $(CALCOBJ)

clean: $(CALCOBJ)
  rm -f AppCalc $(CALCOBJ)
