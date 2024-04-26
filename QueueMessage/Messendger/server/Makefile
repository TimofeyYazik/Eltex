CC := gcc
CFLAGS := -g 
LIB := -lcurses -pthread

FILES := $(shell find . -name "*.c")
OBJECTS := $(patsubst %.c, %.o, $(FILES))

%.o: %.c
	$(CC) $(CFLAGS)  -c $< -o $@

server: $(OBJECTS)
	$(CC) $(CFLAGS) $(LIB) $(OBJECTS) -o $@ 

clean:
	rm -f *.o server $(OBJECTS)

