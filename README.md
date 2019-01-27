# proxasm

A C/x86 assembly implementation of some proximal operators with SSE ([3]) SIMD instructions, you can compile it using the Makefile. (with NASM and GCC). 

# Proximal operators 

A lot of convex optimization problems such as Basis Pursuit, Lasso or Quadratic Programming can be solved using proximal methods ([1], [2]). The proximal operator of a function is defined by

![alt text](https://latex.codecogs.com/gif.latex?%5Ctext%7Bprox%7D_%7Bf%7D%5Cleft%28x%20%5Cright%20%29%20%3D%20%5Carg%5Cmin_%7By%20%5Cin%20%5Cmathbb%7BR%7D%5En%7D%20f%28y%29%20&plus;%20%5Cfrac12%20%5Cleft%5CVert%20y-x%20%5Cright%5CVert_2%5E2)
If the function is convex, the proximal operator is well-defined. Some numerical optimization algorithms like ADMM and proximal gradient methods make extensive use of these operators, they can solve regularized statistical problems like Lasso or Ridge regression and other machine learning methods like SVR. 




# References 

[1] : https://arxiv.org/abs/0912.3522
[2] : https://web.stanford.edu/~boyd/papers/pdf/prox_algs.pdf
[3] : http://softpixel.com/~cwright/programming/simd/sse.php (SSE instruction set) 
[4] : http://proximity-operator.net/ (implementation in Matlab and Python)
