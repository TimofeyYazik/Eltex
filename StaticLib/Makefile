CFLAGS := -std=c11 -Wall -Wextra -Werror

LIBFILES = $(wildcard StaticLib/*.c)
LIBOBJS = $(LIBFILES:.c=.o)

all: calc

%.o: %.c
	gcc $(CFLAGS) -c -g $< -o $@

calclib.a: $(LIBOBJS)
	ar rcs $@ $(LIBOBJS)

calc: main.o calclib.a
	gcc $(CFLAGS) main.o -o calc

clean:
	rm -rf StaticLib/*.o calclib.a calc *.o

clang-format:
	clang-format --style=Google -n *.c *.h CalcLib/*.c