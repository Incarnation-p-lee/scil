# Automaticaly generated by script/produce_compile_makefile.pl.

include /home/pli/workspace/scil/src/Makefile.in
CFLAG += -I/home/pli/workspace/scil/src/inc

TARGET = log.o
all:$(TARGET)

.s.o:
	nasm $(AFLAG) -o $@ $<
log.o:src/log/log.c
	cc $(CFLAG) -o $@ $<
