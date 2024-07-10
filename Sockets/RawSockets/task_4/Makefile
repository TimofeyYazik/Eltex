%.o: %.c
	gcc -g -c $< -o $@

all: server client

server: server.o 
	gcc -pthread $^ -o server 

client: client.o
	gcc -pthread $^ -o client

clean:
	rm -rf server client *.o 
