CFLAGS := -std=c11 -Wall -Wextra

all: obg
	gcc $(CFLAGS) *.o -o abonents_app

obg: 
	gcc -c -g $(CFLAGS) *.c

clean:
	rm -f abonents_app *.o

git:
	git add .
	git commit
	git push