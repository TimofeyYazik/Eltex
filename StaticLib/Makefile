CFLAGS := -std=c11 -Wall -Wextra -Werror

LIBFILES = $(wildcard StaticLib/*.c)

all: calc

libcalc.a: objects
	ar rcs $@ CalcLib/*.o

objects:
	gcc -c -g $(CFLAGS) $(LIBFILES)

calc: main.o libcalc.a
	gcc $(CFLAGS) main.o -L. -lcalc -o $@

clean:
	rm -rf StaticLib/*.o libcalc.a calc *.o

clang-format:
	clang-format --style=Google -n *.c *.h libcalc/*.c