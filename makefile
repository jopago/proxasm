CC=gcc
AS=nasm

all: asm testc

asm: 
	$(AS) SSE/proxl1.s -o proxl1.o -f elf64
	$(AS) SSE/proxl2.s -o proxl2.o -f elf64
	$(AS) SSE/utils.s -o utils.o -f elf64
	$(AS) SSE/projectors.s -o projectors.o -f elf64
	$(AS) SSE/vapnik.s -o vapnik.o -f elf64

	$(AS) AVX/avx_utils.s -o avx_utils.o -f elf64
testc: 
	$(CC) test.c *.o -o test -lm -O3 -I./

clean:
	rm *.o