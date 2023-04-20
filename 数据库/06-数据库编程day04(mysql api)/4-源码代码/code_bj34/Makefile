#获取当前目录下所有的.c文件
SRC:=$(wildcard ./*.c)

#将所有的.c替换成对应的文件名 test.c--> test
BIN:=$(patsubst %.c, %, $(SRC))

LIBPATH:=/usr/lib64/mysql/
INC:=/usr/include/mysql/
LIB:= mysqlclient

all:$(BIN)
%:%.c
	gcc $< -L$(LIBPATH) -l$(LIB)  -o $@

.PHONY:clean
clean:
	$(RM) $(BIN)
