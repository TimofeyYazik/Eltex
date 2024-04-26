CC := gcc
CFLAGS := -g 
LIB := -lcurses -pthread -lrt

FILES := $(shell find . -name "*.c")
OBJECTS := $(patsubst %.c, %.o, $(FILES))

%.o: %.c
	$(CC) $(CFLAGS)  -c $< -o $@

client: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIB) -o $@ 

clean:
	rm -f *.o client $(OBJECTS)