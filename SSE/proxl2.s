section .text:

global sse_proxl2square, sse_proxl2

extern sse_norm2

sse_proxl2square:
	; modifies array in-place
	; copy of original array is done in C

	; prox of 0.5*lambda*||.||_2^2 is simply 
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

sse_proxl2:
	; proximal operator of the euclidean norm 
	; prox_{lambda ||.||_2}(x) = (1 - lambda/max(||x||,lambda)) * x

	movapd xmm3, xmm0 ; save lambda as xmm3

	push rdi 
	call sse_norm2 
	pop rdi 

	; compute (1 - lambda / (max(||x||,lambda))) 

	maxsd xmm0,xmm3  ; xmm0 = max(||x||, lambda)
	divsd xmm3,xmm0  ; xmm3 = lambda / xmm0 

	push rcx
	mov ecx, 1
	cvtsi2sd xmm2,ecx
	subsd xmm2, xmm3 
	movlhps xmm2,xmm2 ; xmm2 = constant to multiply x with 

	pop rcx 

	xor rax,rax
	_proxl2:
		movapd xmm0,[rdi]
		mulpd xmm0,xmm2
		movapd [rdi],xmm0 

		add rdi,16
		add rax,2

		cmp rax,rsi
		jl _proxl2

	ret 