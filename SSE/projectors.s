global sse_projl2
global sse_normalize 

extern sse_norm2

section .text:


sse_normalize: 
	
	push rdi 
	call sse_norm2 
	pop rdi 

	push rcx
	mov ecx,1
	cvtsi2sd xmm2,ecx
	divsd xmm2,xmm0 ; 1/||x||_2 
	movlhps xmm2,xmm2  ; packed
	pop rcx ; restore rcx 

	xor rax,rax

	_normalize_loop:

		movapd xmm0,[rdi]
		mulpd xmm0,xmm2
		movapd [rdi],xmm0

		add rdi,16
		add rax,2

		cmp rax,rsi
		jl _normalize_loop

	
	ret

sse_projl2:
	; projection of vector on l2 ball of center 0 and radius r 
	; if ||x|| < r return x, else return r*x/||x||
	
	movapd xmm3,xmm0 ; save r 
	push rdi 
	call sse_norm2 
	pop rdi 

	; norm2 is in xmm0 

	comisd xmm0,xmm3 ; is norm < r ?
	jb _done  ; 

	; divide by norm 
	push rcx
	mov ecx,1
	cvtsi2sd xmm2,ecx ; xmm2 = 1

	

	divsd xmm2,xmm0 ; 1/||x||
	mulsd xmm2,xmm3 ; r / ||x||
	movlhps xmm2,xmm2 ; xmm0 = [r/||x|| | r/||x||] 

	; now we can loop 
	xor rax,rax

	_project: 
		movapd xmm1,[rdi]
		mulpd xmm1,xmm2
		movapd [rdi],xmm1

		add rdi,16
		add rax,2

		cmp rax,rsi 
		jl _project

	pop rcx

	_done:
		ret 

