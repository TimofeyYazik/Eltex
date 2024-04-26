CC := gcc
CFLAGS := -g -Werror -Wextra  

FILES := $(shell find . -name "*.c")
OBJECTS := $(patsubst %.c, %.o, $(FILES))

%.o: %.c
	$(CC) $(CFLAGS)  -c $< -o $@

shell: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ 

clean:
	rm -f shell $(OBJECTS)

format:
	clang-format --style=Google -i *.c
