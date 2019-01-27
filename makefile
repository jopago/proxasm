CC=gcc
AS=nasm

all: asm testc

asm: 
	$(AS) ../ASM/proxl1.s -o proxl1.o -f elf64
	$(AS) ../ASM/proxl2.s -o proxl2.o -f elf64
	$(AS) ../ASM/utils.s -o utils.o -f elf64

testc: 
	$(CC) test.c *.o -o test -lm -O3
