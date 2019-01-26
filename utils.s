global mul_scalar
global sum 

section .text:

sum:
	; sum of a double array

	xorpd xmm0,xmm0 
	xor rax,rax ; counter 
	shr rsi,1 ; rsi /= 2

	_sum:

		movapd xmm1, [rdi]  ; reads 2 doubles 
		
		haddpd xmm2,xmm1 ; store the sum into xmm2
		haddpd xmm0,xmm2 ; add xmm2 to xmm0

		add rdi,16 ; move to next 2 doubles 
		inc rax ; increase counter

		cmp rax,rsi 
		jne _sum 

	punpckhqdq xmm0,xmm0

	ret