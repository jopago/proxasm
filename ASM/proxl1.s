global asm_proxl1

section .text:

asm_proxl1:
	; proximal operator of l1 norm ("soft-thresholding" operator)
	; prox_{k ||.||_1} (x) = sgn(x)*max(|x| - k, 0)
	; it is separable so we can leverage SSE 

	xor rax,rax
	xorpd xmm2,xmm2
	xorpd xmm3,xmm3 ; xmm3 will store constant [0 | 0]


	_loop:
		subpd xmm2,[rdi]
		maxpd xmm2,[rdi] 	; xmm2 = [abs(x_i+1) | abs(x_i)]
		subpd xmm2,xmm0 	; minus lambda 
		maxpd xmm2,xmm3 	; xmm2 = max(|x| - k, 0)

		; multiply by sign 
		
		add rdi,16
		add rax, 2

		cmp rax,rsi
		jl _loop

	pop rcx

	ret 
