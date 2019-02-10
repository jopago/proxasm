; Vapnik proximal operator
; f(x) = max(||x||_2 - epsilon, 0) 

global sse_vapnik 

extern sse_norm2
extern sse_projl2
extern sse_proxl2 

section .text:

sse_vapnik:
    ; the prox of lambda f is:
    ;       identity if ||x|| <= epsilon
    ;       projl2(x,epsilon) if ||x|| <= epsilon+lambda
    ;       proxl2(x,lambda) otherwise

    movapd xmm3,xmm0 ; epsilon 
    movapd xmm4,xmm1 ; lambda 

    push rdi 
    call sse_norm2
    pop rdi

    comisd xmm0,xmm3 
    jbe _sse_vapnik_identity 

    subsd xmm0,xmm3 
    comisd xmm0,xmm4 ; ||x||-epsilon <= lambda ?
    jbe _sse_vapnik_projl2 

    movapd xmm0, xmm4 
    call sse_proxl2
    ret 

    _sse_vapnik_identity:
        ret 

    _sse_vapnik_projl2:
        movapd xmm0, xmm3 
        call sse_projl2
        ret 