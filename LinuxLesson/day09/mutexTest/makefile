#package all .c files int this directory and assign it to src
src = $(wildcard ./*.c)

#change the postfix .c to executable files
target = $(patsubst %.c, %, $(src))

CC = gcc 

CFLAGS = -lpthread
LDFLAGS = 

all:$(target)

%:%.c
	$(CC) $^ -o $@ $(CFLAGS)

clean:$(target)
	rm $^ -f

.PHONY:all clean
