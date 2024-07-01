all: server client

client: client.o
	gcc $^ -o client

server: server.o
	gcc $^ -o server

clean:
	rm -rf server client *.o
