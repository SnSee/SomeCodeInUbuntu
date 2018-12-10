#target:dependency
#(tab)command
#(tab)command
#...
all:main

src=$(wildcard ./*.c)

obj=$(patsubst %.c,%.o,$(src))

main:$(obj)
	gcc $^ -o main

%.o:%.c
	gcc -c $< -o $@

clean:
	rm -f $(obj)
	rm main -f

.PHONY:clean all
