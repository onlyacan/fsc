/* header file: inc/ssms.h
 * 1. define the data structure of sparse martix: crs
 * 2. method bicgstab to solve it.
*/


# include "ssms.h"
# include <math.h>

/* N: dimension of the matrix A
 * NNZ: number of non-zero elements
 * return: pointer to the spare matrix A
 *
 */

sm* sm_constr(id n, id nnz)
{
    sm *A = (sm*) malloc(sizeof(sm) * 1);
    A->n = n;
    A->nnz = nnz;
    A->data = (real *) smalloc(sizeof(real) * A->nnz, "In sm_constr: for A.data");
    A->col_ind = (id *) smalloc(sizeof(id) * A->nnz, "In sm_constr: for A.col_ind");
    A->row_ptr = (id *) smalloc(sizeof(id) * (A->n + 1), "In sm_constr: for A.row_ptr");
    A->row_ptr[0] = 0;
    A-> top = 0; 
    
    return A;
}

/* Assign value to sparse matrix A
 * input: 
 * 	A: sparse matrix 
 * 	i: ith row, starting from 0 to n-1
 * 	js: j index for the non-zero elements in ith row
 * 	datas: non-zero datas in ith row
 * 	nnzi: number of non-zero elements in row i
 *      tested 
 * !!! this is a very unsafe function, please not use too much.
*/

void sm_assign(sm *A, id i, id *js, real *datas, id nnzi)
{
    id k;
    id l;

    if (nnzi <= 0) exit(-1);
    

    // top of the array 
    k = A->row_ptr[i] = A-> top; 
    A->row_ptr[i+1] = A->top = A-> top + nnzi;
    
    for (l=0; l < nnzi; l++){	
	A-> data[k+l] = datas[l];
	A-> col_ind[k+l] = js[l];
    }
}



/* print the sm matrix 
 * input :
 *    - fp: file pointer
 *    - A: matrix pointer  
 */

void sm_fprint(FILE *fp, sm *A) 
{
    id i,j,k,n;
    
    n = A->n;
    
    id *markers = (id*) smalloc(sizeof(id)* n, "In sm_print(), for markers");
    for(j= 0; j< n; j++){
	markers[j] = -1;
    }
      
    
    for(i=0; i < n; i ++) { // for every line 
	for(k= A-> row_ptr[i]; k< A-> row_ptr[i+1]; k++){
	  j = A-> col_ind[k];
	  markers[j] = k;
	}
	
	for(j=0; j < n; j++){ // loop each col in a row 
	  if (markers[j] != -1) 
	      fprintf(fp, "%10.2le", A->data[markers[j]]);
	  else
	      fprintf(fp, "%10s", "X");
	  
	}

      
        
	fprintf(stdout, "\n"); // after the loop of each line

        for(j= 0; j< n; j++){
            markers[j] = -1;  // restore the values of markers 
        }    

    } // end of looping each row 
    
} // end of function

/*=================================== for sparse matrix operations =======*/

/*
  i th raw vector from sparse matrix A 
  multiply vector x 
  return a double value 
  user take responsibility x vector is larger than the dimension of A 
*/

real sm_row_dot(sm *A, int i, real *x)
{
    real dot;
    id k,len;
    real *data_ptr;    // the data ptr for the ith row
    id   *col_ind_ptr; // the data ptr for the ith row

    data_ptr = A-> data + A-> row_ptr[i];
    col_ind_ptr = A-> col_ind + A-> row_ptr[i];
    len = A-> row_ptr[i+1] - A-> row_ptr[i]; 

    for (k= 0, dot=0.0; k < len; k++) dot += *(data_ptr + k) *  x[*(col_ind_ptr +k)];

    return dot; 
}


/*
 * sparse matrix vector multiplication
 * sparse version of the xGEMV in BLAS
 * y <-- A * x
 * return the y ptr 

*/
real *smv(sm *A, real *x, real *y)
{
    id i; // raw index 
    for(i=0; i< A->n; i ++) y[i] = sm_row_dot(A, i, x);
    return y; 
}


/*
  general sparse_matrix mv - perform one of the matrix-vector operations   
  y := alpha*A*x + beta*y

*/
real *gesmv(id n, real alpha, sm *A, real *x, real beta, real *y)
{
    id i; // raw index 
    for(i=0; i< A->n; i ++) y[i] = alpha * sm_row_dot(A, i, x) + beta * y[i];
    return y; 
}



void sm_lil_install(sm_lil *A, id i, id j, real val)
/*
  install the value to i,j of the matrix A, A(i,j) = val 
  the index i, j start from 0
*/
{
    struct node *tmpNode;
    
    if (i <0 || i >= A-> n || j<0 || j>= A->n ){  // check invalid case
        fprintf(stderr, "Error: in sm_lil_install, index i= %lli, j= %lli not valid in matrix n = %lli\n", i,j,A->n);
        exit(-1);
    }
    else{  // input i,j are valid 
        tmpNode = smalloc(sizeof(struct node) * 1, "in sm_lil_install, for tmpNode");
        tmpNode-> data = val;
        tmpNode-> col_ind = j;

        if (A-> heads[i] != NULL){
            tmpNode-> next = A->heads[i]; 
            A-> heads[i] = tmpNode;
        }
        else{
            A-> heads[i] = tmpNode;
        }

        A-> nnz ++;
    }
}

/* sm_lil_constr: construct the sparse matrix in lil format
 * very line in a sparse matrix A has a pointer to the ith linked list 
*/
sm_lil *sm_lil_constr(id n)
{
    id i;

    sm_lil *A = smalloc(sizeof(sm_lil) * 1, "sm_lil_constr() for A");
    A-> n = n;
    A->heads = smalloc(sizeof(struct node*) * A-> n , "in sm_lil_constr() for A-> heads");
    for(i=0; i< n; i++){
        A->heads[i] = (struct node*) NULL; 
    }
    A->nnz = 0; 

    return A;
}



/*
  print the lil format lil sparse matrix 
*/
void sm_lil_fprint(FILE *fp, sm_lil *A)
{
    id i;
    struct node *nd;

    fprintf(fp, "# dimension: %lli, %lli non-zero elements\n", A-> n, A-> nnz);

    for(i=0; i < A->n; i++){ // each line 
        for (nd = A-> heads[i]; nd != NULL ; nd = nd -> next)
            fprintf(fp, "A(%12lli,%12lli) = %12.4lf\n", i, nd-> col_ind, nd-> data);
    }
}

/*
  convert lil format to csr
  tested 
*/
sm *sm_lil_to_csr(sm_lil *A_lil)
{
    id i,*js, nnz;
    struct node *nd;
    real *datas;
    char progname[] = "in sm_lil_to_csr";
    sm* A;

    A = sm_constr(A_lil->n, A_lil -> nnz);
    js = smalloc(sizeof(id)*A->n, progname);
    datas = smalloc(sizeof(real)*A->n, progname);
    
    for(i=0; i < A_lil-> n; i++){
        if ((nd = A_lil -> heads[i]) != NULL){
            for (nnz = 0; nd != NULL ; nd = nd -> next, nnz++){
                js[nnz] = nd-> col_ind;
                datas[nnz] = nd-> data ;
            }
            sm_assign(A, i, js, datas, nnz);
        }
    }

    return A; 
}


