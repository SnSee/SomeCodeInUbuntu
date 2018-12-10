# By Panda.Guo at 2018-10-18 09:49:03 created


# 功能： 将当前目录下的.c文件，生成同名的可执行文件，
#  如 ： hello.c   --> hello


src = $(wildcard *.c)
target = $(patsubst %.c, %, $(src))

# CC = arm-linux-gcc
CC = gcc

CFLAGS = -Wall -g
LDFLAGS =

all:  $(target)

%:%.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	-rm -rf $(target)


distclean: clean
	-rm *~

.PHONY: all clean distclean
