server: server.o
	gcc $^ -o server 

client: client.o
	gcc -pthread $^ -o client

clean:
	rm -rf server client *.o 
