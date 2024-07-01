%.o: %.c
	gcc -g -c $< -o $@

all: client

client: client.o
	gcc $^ -o client

clean:
	rm -rf client *.o 
