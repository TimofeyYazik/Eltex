FILES := *.c
OBJ := $(patsubst %.c, %.o, $(FILES))

%.o: %.c
	gcc -c -g $< -o $@

all: libmess.a

libmess.a: $(OBJ)
	ar rcs $@ $(OBJ)
	cp $@ ../client
	cp $@ ../server

clean:
	rm -rf *.o libmess.a