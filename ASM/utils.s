global asm_norm2
global asm_sum

section .text:

asm_sum:
	; sum of a double array

	xorpd xmm0,xmm0 
	xor rax,rax ; counter 

	_sum:		
		addpd xmm0,[rdi] 	; adds 2 double at once in xmm0 
					; the top and bottom part of xmm0 contain each approx. half of the sum

		add rdi,16
		add rax,2 
		
		cmp rax,rsi 
		jl _sum 

	haddpd xmm0,xmm0

	ret
	
asm_norm2:
	xorpd xmm0,xmm0 
	xor rax,rax ; counter 

	_sum_square:

		movapd xmm1, [rdi]  ; reads 2 doubles 
		
		mulpd xmm1,xmm1  ; square them 
		addpd xmm0,xmm1

		add rdi,16 ; move to next 2 doubles 
		add rax,2 ; increase counter by 2

		cmp rax,rsi
		jl _sum_square 

	haddpd xmm0,xmm0
	sqrtpd xmm0,xmm0

	ret
