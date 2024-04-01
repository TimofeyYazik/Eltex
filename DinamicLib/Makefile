CFLAGS := -std=c11 -Wall -Wextra -Werror

LIBFILES = $(wildcard CalcLib/*.c)
LIBOBJS = $(patsubst %.c, %.o, $(LIBFILES))

all: calc

objlib: $(LIBOBJS)

libcalc.so: objlib
	gcc $(CFLAGS) -shared -o $@ $(LIBOBJS)

calc: main.o libcalc.so
	gcc $(CFLAGS) main.o -L. -lcalc -o $@
	LD_LIBRARY_PATH=$(PWD):$(LD_LIBRARY_PATH) ./calc

%.o: %.c
	gcc $(CFLAGS) -fPIC -c $< -o $@

clean:
	rm -rf CalcLib/*.o libcalc.so calc *.o

clang-format:
	clang-format --style=Google -n *.c *.h CalcLib/*.c
