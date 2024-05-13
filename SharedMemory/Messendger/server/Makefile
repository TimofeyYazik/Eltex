CC := gcc
CFLAGS := -g 
LIB := -lcurses -pthread -L../lib -lmess -lrt 

FILES := $(shell find . -name "*.c")
OBJECTS := $(patsubst %.c, %.o, $(FILES))

%.o: %.c
	$(CC) $(CFLAGS)  -c $< -o $@

server: $(OBJECTS)
	$(CC) $(CFLAGS)  $(OBJECTS) $(LIB) -o $@ 

clean:
	rm -f *.o server $(OBJECTS)
