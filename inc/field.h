#ifndef __FIELD_H__
#define __FIELD_H__

#include "mesh.h"
#include "error.h"


void calc_cds_fx(mesh *m, real *fxs);

/*
  A ndarray is stored continuesly in a memery
  to total number of element = len = 
 */

# define MAX_RANK 3 



typedef  struct
{
    char *name;     // name of the array
    int  rank;      // scalar has rank 0, vector has 1,  tensor has 2
    int  shape[MAX_RANK];   // now only suppor tensor    
    int  len;
    double *data;     // pointer to the data 

}ndarray;




typedef struct
{
    char *name;          // name of the array
    int  rank;           // scalar has rank 0, vector has 1,  tensor has 2
    int  shape[MAX_RANK];// now only suppor tensor    
    int  n;              // number of element (ndarray) in this field 
    int  len;            // length of all menmenry   
    int  stride;         // length of each element 
    double *data;   // pointer to the data 
    double *old;    // pointer to the data 
    
}ndarrayField;


ndarrayField *ndarrayField_constr(const char *name, const char *shape, int nelem, real init_val);
void ndarrayField_fprint(FILE *fp, const ndarrayField *phi);
void check_if_shape_valid(ndarrayField  *phi);
void ndarrayField_assign(double val, ndarrayField *x);





#endif /*__FIELD_H__*/
