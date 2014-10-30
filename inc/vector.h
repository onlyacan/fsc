#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "type.h"
#include "error.h"
/*
  this module is for vector operation

*/



real *vcopy(id n, real *y, real *x);        /* copy the vector from x to y, y = x*/
real *vdump(id n, real *src);               /* dumplicate a vector src with dimension n*/
real *vassign(id n, real *x, real val);     /* assign a real to x with dimension n */

real *axpy(id n, real a, real *x, real *y); /* y = a*x + y*/ 
real dot(id n, real *a, real *b);           /* doct product of two vector*/
real norm(id n, double *b);                 /*calculate the norm of a vector*/




#endif /*__VECTOR_H__*/
