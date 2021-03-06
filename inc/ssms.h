#ifndef __SSMS_H__
#define __SSMS_H__

# include "type.h"
# include "main.h"
# include "error.h"
# include <stdlib.h>


typedef struct 
{
    real *data;    
    id *col_ind;         // same number as data, each element has a data, and a j-index. 
    id *row_ptr;         // ptr of data, starting of each raw in array data and col_ind, cotinues  
    id n;                // dimension of the matrix 
    id nnz;              // number of non-zero elements 
    id top;              // index of the top element in of the memery  col_ind and data, for assign 
} sm;


struct node 
{
    id col_ind; // col index 
    real data;
    struct node *next; 
};

typedef struct 
{
    id n;                // dimension of the matrix 
    struct node **heads;  // array of head of lines in matrix  
    id nnz;              // number of non-zero element
    
}sm_lil; 



sm  *sm_constr (id n, id nnz);
void sm_assign(sm *A, id i, id *js, real *datas, id nnzi);
void sm_fprint(FILE *fp, sm *A) ; 

// sparse matrix operators 
real sm_row_dot(sm *A, int i, real *x); 
real *smv(sm *A, real *x, real *y); 
real *gesmv(id n, real alpha, sm *A, real *x, real beta, real *y);

// for the sm in lil format
sm_lil *sm_lil_constr(id n);
void sm_lil_install(sm_lil *A, id i, id j, real val);
void sm_lil_fprint(FILE *fp, sm_lil *A);
sm * sm_lil_to_csr(sm_lil *A_lil);

// iteration solvers
int bicgstab(sm *A, real *x, real *b,  real *work, int max_it, int *iter, real tol, real *error);
/*
  n - dimension of the vector of b and x, user take charge of its correctness
  return the flag of the solver 
  output to pointer:
      - x : the ouput value 
      - error: real error norm
      - iter: actually ferformed iterations
      flag:
         0 = solution found to tolerance
         1 = no convergence given max_it
        -1 = breakdown: rho = 0
        -2 = breakdown: omega = 0
*/

#endif 
