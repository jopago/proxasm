CC			= gcc
ASM 		= nasm
CPP 		= g++
EIGENPATH	= /Users/joan/Downloads/Eigen/

all: asm testc

asm: 
	$(AS) SSE/proxl2.s -o proxl2.o -f macho64
	$(AS) SSE/utils.s -o utils.o -f macho64
	$(AS) SSE/projectors.s -o projectors.o -f macho
	$(AS) SSE/vapnik.s -o vapnik.o -f macho
testc: 
	$(CC) test.c *.o -o test -lm -O3 -I./ -msse3 -mavx 

test_eigen:
	$(CPP) eigen_test.cpp -mavx -O3 -I$(EIGENPATH) -o test_eigen 

clean:
	rm *.o test test_eigen
