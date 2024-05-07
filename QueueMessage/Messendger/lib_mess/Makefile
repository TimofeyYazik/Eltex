

%.o: %.c
	gcc -c -g $< -o $@

all: libmess.a

obj:
	gcc -c -g *.c

libmess.a: obj
	ar rcs $@ *.o

clean:
	rm -rf *.o libmess.a