CC := gcc
CFLAGS := -g 
LIB := -lcurses

FILES := $(shell find . -name "*.c")
OBJECTS := $(patsubst %.c, %.o, $(FILES))

%.o: %.c
	$(CC) $(CFLAGS)  -c $< -o $@

messly: $(OBJECTS)
	$(CC) $(CFLAGS) $(LIB) $(OBJECTS) -o $@ 

clean:
	rm -f *.o messly $(OBJECTS)