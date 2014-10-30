#include  "ssms.h"
#include  "main.h"
#include  "vector.h"
#include  "type.h"
#include  <time.h>

#include <stdio.h>
#include <stdlib.h>



int main(void)
{
    sm *A;                /* the matrix */  
    sm_lil *A_lil;        /* the matrix in lil format*/
    id n,nnz_raw_max; /* n: dimension, nnz number of non-zero*/
    id i; 
    real a; 
    time_t t;
    real *b, *x, *x0; 
    real *work;
    int print_flag;
    FILE *fp ;

    int max_it = 1000;
    int iter = 0;
    double tol = 1.0e-2 ;
    double error = 0.0;
    int flag = 20;

    printf("n(dimension), nnz_raw_max(max nz elem every line), tol, print flag(0:not print): \n");
    scanf("%lli %lli %lf %d", &n, &nnz_raw_max, &tol, &print_flag);
    printf("n=%lli, nnz_raw_max = %lli\n", n, nnz_raw_max);
    

    printf("First construct a lil sparse matrix\n");
    A_lil = sm_lil_constr(n);
    printf("dimension of the sparse matrix = %lli\n", A_lil -> n);
    printf("assign variable\n");



    for(i=0, srand((unsigned) time(&t)); i < n; i++){
        a = (rand()%1000)/1000.0 * 12345.0;
        sm_lil_install(A_lil, i,i, a);
    }

    printf("convert A_lil to A\n");
    A = sm_lil_to_csr(A_lil);
    // printf("then print A:\n");
    // sm_fprint(stdout, A); 

    printf("create vector b and x");    
    b = (real*) malloc(sizeof (real) * n);  
    x = (real*) malloc(sizeof (real) * n);


    printf("assign initial value for x\n");      
    for(i=0, srand((unsigned) time(&t)); i < n; i++){
        x[i] = (rand()%1000)/1000.0 * 12345.0;
    }




    printf("make vector b\n");
    smv(A, x, b); 
    printf("b:\n");
    /*
    for(i=0; i < n; i++){
        printf("%12.2lf\n", b[i]);
    }
    */
    

    x0 = vdump(n,x);


    printf("clearn x ==> x0, x\n"); 
    vassign(n,x,0.0);

    printf("%12s%12s%12s\n","x0", "b", "x"); 
    /*
    for(i=0; i < n; i++){
        printf("%12.2le%12.2le%12.2le\n", x0[i], b[i], x[i]);
    }
    */
    /*
    for(i=0; i < n; i++){
        printf("%12.2lf, %12.2lf\n", x0[i], x[i]);
    }
    */


    

    work = (real*) smalloc(sizeof(real) * A->n*8, "in test_bicgstab for work");
    flag = bicgstab(A, x, b, work, max_it, &iter, tol, &error);
    

    printf("after solution flag=%i, after %i iteration, error = %lf, solution x:\n", flag, iter, error);
    if (flag != 0){
        fprintf(stderr, "NOT converge\n");
    }else{
        fp = fopen("result_test_bicgstab.data", "w");
        if (print_flag != 0)
            for(i=0; i < n; i++)
                fprintf(fp, "%12.2lf, %12.2lf, %12.2lf\n", x0[i], x[i], x[i]-x0[i]);
        
    }

    fclose(fp);
    
    

    return 0;
}
