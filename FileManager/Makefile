
CC = gcc
FLAGS = -g
LIB = -lncurses

SOURCES := $(wildcard $(shell find . -name '*.c'))
OBJ = $(patsubst %.c, %.o, $(SOURCES))

%.o: %.c
	$(CC) $(FLAGS) -c $< 

all: $(OBJ)
	$(CC) $(FLAGS) *.o $(LIB) -o FIM

clean:
	rm -rf FIM *.o 

format:	$(SOURCES)
	clang-format --style=Google -i $(SOURCES)

git:
	git add .
	git commit
	git push

