# Automaticaly generated by script/produce_compile_makefile.pl.

include /home/pli/workspace/scil/src/Makefile.in
CFLAG += -I/home/pli/workspace/scil/src/inc

TARGET = token.o
all:$(TARGET)

.s.o:
	nasm $(AFLAG) -o $@ $<
token.o:src/tokenizer/token/token.c
	cc $(CFLAG) -o $@ $<
