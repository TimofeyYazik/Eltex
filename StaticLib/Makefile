CFLAGS := -std=c11 -Wall -Wextra -Werror

LIBFILES = $(wildcard CalcLib/*.c)
LIBOBJS = $(LIBFILES:.c=.o)

all: calc

%.o: %.c
	gcc $(CFLAGS) -c -g $< -o $@

libcalc.a: $(LIBOBJS)
	ar rcs $@ $(LIBOBJS)

calc: main.o libcalc.a
	gcc $(CFLAGS) main.o -L. -lcalc -o $@

clean:
	rm -rf CalcLib/*.o libcalc.a calc *.o

clang-format:
	clang-format --style=Google -n *.c *.h libcalc/*.c