%.o: %.c
	gcc -g -c $< -o $@

all: server client

server: server.o double_list/list.o
	gcc -pthread $^ -o server 

client: client.o
	gcc $^ -o client

clean:
	rm -rf server client *.o 
