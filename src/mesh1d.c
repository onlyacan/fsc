#include "mesh.h"
#include "main.h"


real *mesh1d(real xmin, real xmax, id nx, real exx)
{
  real dx ; 
  id   np ;
  id   i, ipc ;
  real *x = NULL;
  
  if(nx%2 != 0) nx = nx + 1;// nx must be even
  np = nx + 1;
  ipc = np / 2;   //ipc: center 
  x = malloc(sizeof(real) * np);
  if (exx ==1.0) dx = (xmax - xmin) / nx ;
  else dx = (xmax -xmin)/2 * (1.0 - exx) / (1.0 - pow(exx, np/2));

  x[0] = xmin;
  for (i=1; i < ipc; i++) {x[i] = x[i-1] + dx; dx = dx* exx;} 
  x[ipc] = (xmax - xmin) / 2.0 ;
  for (i=ipc+1; i < np-1; i++) {x[i] = x[i-1] + dx; dx = dx / exx;}
  x[np-1] = xmax;
  
  return x;
}
