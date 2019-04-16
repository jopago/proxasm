#ifndef _SSE_H
#define _SSE_H

extern double 	sse_norm2(const double*,const int);
extern double 	sse_norm1(const double*,const int);
extern double 	sse_sum(const double*,const int);


extern void 	sse_abs(double*,const int);
extern void 	sse_normalize(double*,const int);
extern void 	sse_projl2(double*,const int,double);
extern void 	sse_projlinf(double*,int,double);

extern void 	sse_proxl2(double*,const int,double);
extern void 	sse_proxl2square(double*,int,double);
extern void 	sse_proxl1(double*,int,double);
extern void 	sse_vapnik(double*,int,double,double);

#endif
