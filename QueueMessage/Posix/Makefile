server: server.o
	gcc -g $< -lrt -o $@

client: client.o
	gcc -g $< -lrt -o $@

clean:
	rm -f *.o server client