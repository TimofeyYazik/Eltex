%.o: %.c
	gcc -g -c $< -o $@

all: server tcp_client udp_client

server: server.o 
	gcc -pthread $^ -o server 

tcp_client: tcp_client.o
	gcc $^ -o tclient
udp_client: udp_client.o
	gcc $^ -o uclient

clean:
	rm -rf server uclient tclient *.o 
