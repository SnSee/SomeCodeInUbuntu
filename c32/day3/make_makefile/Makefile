

# 代表注释

#   目标： 依赖1 依赖2
#      	（tab） 命令列表
# 		（tab）....


# 变量

#src = add.c sub.c main.c mul.c
src := add.c

# obj = add.o sub.o main.o mul.o

obj = $(patsubst %.c, %.o, $(src))

#CFLAGS=-I ./  -I ./include -c
a:
	echo $(src)
all: main
	echo $(src)
main : $(obj)
	gcc $^ -o main 

%.o:%.c
	gcc -c $< -o $@

src := a.c
# add.o : add.c add.h 
# 	gcc $(CFLAGS) $< -o $@

b:
	echo $(src)
# sub.o:sub.c
# 	gcc -c $< -o $@

# mul.o:mul.c 
# 	gcc -c $< -o $@
# main.o: main.c
# 	gcc -c $< -o $@

clean:
	rm -f $(obj)
	rm -f main




# 伪目标
.PHONY:  all clean
