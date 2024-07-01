%.o: %.c
	gcc -g -c $< -o $@

all: client

client: client.o
	gcc -pthread $^ -o client

clean:
	rm -rf client *.o 
