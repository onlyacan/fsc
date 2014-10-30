#include "vector.h"
#include <math.h>


/*
  vector copy 
  y = x
*/
real *vcopy(id n, real *y, real *x)
{
    id i;
    for(i=0; i<n; i++)  y[i] = x[i]; 
    return y; 
}


/* dumplicate a vector src with dimension n*/
double *vdump(id n, double *src)
{
    id i;
    real *b; 
    b = (real *) smalloc(sizeof(real) * n, "in array_dump() for b");
    for(i=0;i<n;i++) *(b+i) = *(src+i);
    return b;
}

/* assign */
/*     val -->x */
/* assign a real to x with dimension n */
real *vassign(id n, real *x, real val)
{   
    id i;
    for(i=0; i < n; x[i++] = val) ;
    return x;
}



/*
  return  dot product of two vector 

*/
real dot(id n, real *a, real *b)
{
    int i;
    real dot;
    for(i=0, dot = 0.0; i<n; i++) dot += a[i] * b[i]; 
    return dot; 
}



/*calculate the norm of a vector*/
real norm(id n, double *b)
{
    real dotp;
    int i;
    for(i=0, dotp =0.0; i<n; i++) dotp += (b[i] * b[i]); 
    return sqrt(dotp);
}


/*
  scalar a x plus y
  n- dimension of the vector a,x,y
  y = y + ax 
*/

real *axpy(id n, real a, real *x, real *y)
{
    id i;
    for(i=0; i< n; i ++) y[i] += a * x[i];
    return y; 
}




