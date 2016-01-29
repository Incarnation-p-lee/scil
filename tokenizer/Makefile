TAR = tokenizer.elf
OBJ = main.o tokenizer.o
CC  = gcc

all:$(TAR)

$(OBJ):%.o:%.c
	$(CC) -c -g -o $@ $<

$(TAR):$(OBJ)
	$(CC) -o $@ $^

clean:
	rm -v *.o

