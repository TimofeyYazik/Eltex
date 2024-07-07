%.o: %.c
	gcc -g -c $< -o $@

all: server tcp_client udp_client

server: server.o double_list/list.o
	gcc -pthread $^ -o server 

tcp_client: tcp_client.o
	gcc $^ -o uclient
udp_client: udp_client.o
	gcc $^ -o tclient

clean:
	rm -rf server client *.o 
