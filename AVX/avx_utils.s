global avx_sum

avx_sum:
	xor rax,rax
	vxorpd ymm2,ymm2 

	_avx_sum:
		vaddpd ymm2, [rdi] ; sum 4 by 4 double

		add rdi, 32
		add rax, 4

		cmp rax, rsi
		jl _avx_sum

	; horizontal sum of 256-bit AVX register is not obvious 

	vhaddpd ymm2,ymm2 	; top and bottom of ymm2 contains two double each 
						; which correspond to respective top and bottom horizontal sum
	vextractf128 xmm0,ymm2,0 ; extract bottom double-quadword 
	vextractf128 xmm1,ymm2,1 ; extract top 
	addpd xmm0,xmm1 

	ret 