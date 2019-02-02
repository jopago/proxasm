global sse_norm2, sse_norm1
global sse_sum, sse_abs
global sse_test

section .text:

sse_sum:
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
	
sse_norm2:
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
	
sse_abs:
	; |x| = max(-x,x)..
	
	xor rax,rax

	_loop_abs:
		xorpd xmm1,xmm1 ; xmm1 = [0 | 0]
		
		subpd xmm1,[rdi] ; xmm1 = [-x1 | -x0]
		maxpd xmm1,[rdi]
		movapd [rdi],xmm1

		add rdi,16
		add rax, 2

		cmp rax,rsi
		jl _loop_abs

	ret 

sse_norm1: 
 	xor rax,rax
	xorpd xmm0,xmm0

	_loop_norm1:
		xorpd xmm1,xmm1 
		subpd xmm1,[rdi] 
		maxpd xmm1,[rdi]

		addpd xmm0,xmm1

		add rdi,16
		add rax,2

		cmp rax,rsi
		jl _loop_norm1

	haddpd xmm0,xmm0

	ret

sse_test: 
	mov ecx,1
	cvtsi2sd xmm2,ecx

	comisd xmm0,xmm2
	jb _end

	movapd xmm0,xmm2

	_end:
		ret