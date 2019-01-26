section .text:

global sum
global norm2
; global test_arg

norm2:
	xorpd xmm0,xmm0 
	xor rax,rax ; counter 
	shr rsi,1 ; rsi /= 2

	_sum_square:

		movapd xmm1, [rdi]  ; reads 2 doubles 
		
		mulpd xmm1,xmm1  ; square them 
		haddpd xmm2,xmm1 ; store the sum into xmm2
		haddpd xmm0,xmm2 ; add xmm2 to xmm0

		add rdi,16 ; move to next 2 doubles 
		inc rax ; increase counter

		cmp rax,rsi
		jne _sum_square 

	punpckhqdq xmm0,xmm0
	sqrtpd xmm0,xmm0

	ret

sum:
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