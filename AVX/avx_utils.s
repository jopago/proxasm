global avx_sum

avx_sum:
	xor rax,rax
	vxorpd ymm2,ymm2 

	_avx_sum:
		vaddpd ymm2, [rdi] ; sum 4 double at a time

		add rdi, 32
		add rax, 4

		cmp rax, rsi
		jl _avx_sum

	; horizontal sum of 256-bit AVX register is not obvious 
	; let ymm2 = [a | b | c | d]

	vhaddpd ymm2,ymm2 ; ymm2 = [a+b | a+b | c+d | c+d ]
				
	vextractf128 xmm0,ymm2,0 ; extract bottom double-quadword (c+d)
	vextractf128 xmm1,ymm2,1 ; extract top  (a+b)
	addpd xmm0,xmm1 ; xmm0 <- (a+b+c+d) 
	
	ret 
