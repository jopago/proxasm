; not done yet 
; 

global asm_proxl1

section .text:

asm_proxl1:
	xor rax,rax
	xorpd xmm2,xmm2

	push rcx

	mov rcx,1
	shl rcx,63

	cvtsi2sd xmm2,rcx

	;psrlq xmm2,1
	movlhps xmm2,xmm2  


	_loop:
		movapd xmm1,[rdi]

		andnpd xmm1,xmm2 ; absolute value 

		movapd [rdi],xmm1

		add rdi,16
		add rax, 2

		cmp rax,rsi
		jl _loop

	pop rcx

	ret 
