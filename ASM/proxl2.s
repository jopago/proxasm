section .text:

global asm_proxl2square

asm_proxl2square:
	; modifies array in-place
	; copy of original array is done in C
	; prox of ||.||_2^2 is simply 
	; scalar multiplication by 1/(1+lambda) 


	xor rax,rax
	xorpd xmm2,xmm2

	push rcx ; save rcx
	mov ecx,1
	cvtsi2sd xmm2,ecx ; xmm2 = 1

	addpd xmm0,xmm2 ; 1+lambda
	divpd xmm2,xmm0 ; 1/(1+lambda)

	movlhps xmm2,xmm2 ; copy bottom to top
	
	; now xmm2 = (1/(1+lambda) | 1/(1+lambda))

	_loop: 
		movapd xmm1, [rdi]
		mulpd xmm1,xmm2 

		movapd [rdi],xmm1 ; in-place modification

		add rdi, 16
		add rax,2

		cmp rax, rsi
		jl _loop

	pop rcx 

	ret 


