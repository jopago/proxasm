global sse_projl2
global sse_normalize 

extern sse_norm2

section .text:


sse_normalize: 
	xor rax,rax
	call sse_norm2 

	push rcx
	mov ecx,1
	cvtsi2sd xmm2,ecx
	divsd xmm2,xmm0 ; 1/||x||_2 

	movlhps xmm2,xmm2  ; packed

	_normalize_loop:

		movapd xmm0,[rdi]
		mulpd xmm0,xmm2
		movapd [rdi],xmm0

		add rdi,16
		add rax,2

		cmp rax,rsi
		jl _normalize_loop

	pop rcx
	ret

sse_projl2:
	; projection of vector on l2 ball of center 0 and radius r 
	; if ||x|| < r return x, else return r*x/||x||
	
	movapd xmm3,xmm0 ; save xmm0 
	call sse_norm2 

	; norm2 is in xmm0 

	ucomisd xmm0,xmm3 ; is norm < r ?
	jl _done 

	xor rax,rax


	_done:
		ret 

