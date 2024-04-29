client: client.o
	gcc -std=c11 $^ -o $@

client.o: client.c
	gcc -std=c11 -g -c $^ -o $@

server: server.o
	gcc -std=c11 $^ -o $@

server.o: server.c
	gcc -std=c11 -g -c $^ -o $@

clean:
	rm -f *.o client server