CFLAGS := -std=c11 -Wall -Wextra -Werror

LIBFILES = $(wildcard CalcLib/*.c)

all: calc

objlib: $(LIBFILES)
	gcc -c -fPIC $(CFLAGS) $(LIBFILES)

libcalc.so: objlib
	gcc $(CFLAGS) -shared CalcLib/*.o -o $@  

calc: main.o libcalc.so
	gcc $(CFLAGS) main.o -L. -lcalc -o $@
	LD_LIBRARY_PATH=$PWD:$LD_LIBRARY_PATH ./calc

clean:
	rm -rf CalcLib/*.o libcalc.so calc *.o

clang-format:
	clang-format --style=Google -n *.c *.h libcalc/*.c