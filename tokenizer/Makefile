TAR = tokenizer.elf
OBJ = main.o tokenizer.o
CC  = gcc

all:$(TAR)

$(OBJ):%.o:%.c
	$(CC) -c -g -Wall -Werror -DX86_64 -o $@ $<

$(TAR):$(OBJ)
	$(CC) -o $@ $^ ../lib/libds.a -lm
	rm -vf *.o

clean:
	rm -vf *.o

