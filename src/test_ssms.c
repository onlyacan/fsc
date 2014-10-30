#include <stdio.h>
#include "ssms.h"



int main(void)
{
    sm *A;
    sm_lil *A_lil;
    id n;
    id nnz;
    id js[3] = {0,1};
    real data[3] = {1.8, 1.9};
    int i,j;


    printf("test ssms\n");
    printf("1. test %s\n", "sm_assign and sm_fprint");
    n = 3; nnz = 6;
    A = sm_constr(n, nnz);
    sm_assign(A, 0, js, data, 2);
    js[0] = 1; data[0] = 5.6;
    sm_assign(A, 1, js, data, 1);
    sm_fprint(stdout, A);
    puts("\n");



    /* test the  sm_lil*/
    printf("2. test %s\n", "sm_lil_install and sm_fprintf");
    n = 10;
    A_lil = sm_lil_constr(n);

    sm_lil_install(A_lil, 2,9, 29.0);
    sm_lil_install(A_lil, 3,4, 34.0);
    sm_lil_install(A_lil, 5,0, 50.0);
    sm_lil_install(A_lil, 9,2, 92.0);
    sm_lil_install(A_lil, 8,3, 83.0);
    sm_lil_install(A_lil, 6,6, 66.0);
    sm_lil_fprint(stdout, A_lil);

    printf("convert A_lil to A\n");
    A = sm_lil_to_csr(A_lil);
    printf("then print A:\n");
    sm_fprint(stdout, A); 


    return 0;
}
