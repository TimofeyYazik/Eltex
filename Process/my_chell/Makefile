CC := gcc
CFLAGS := -g 

FILES := $(shell find . -name "*.c")
OBJECTS := $(patsubst %.c, %.o, $(FILES))

%.o: %.c
	$(CC) $(CFLAGS)  -c $< -o $@

shell: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ 

clean:
	rm -f *.o shell $(OBJECTS)