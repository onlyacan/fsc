#ifndef __EQUATION_H__
#define __EQUATION_H__

#include "mesh.h"
#include "error.h"
#include "ssms.h"
#include "control.h"

#include <string.h>
#include <stdlib.h>




struct equation {
    char *name;
    mesh *mesh;
    sm *A; 
    sm_lil *A_lil; 
    real *b;
    innerIterCtrl *iic; // inner iteration  control 
};

typedef struct equation Equation;



Equation *equation_constr(mesh *m, const char *name, int strid);
void update_linsys_Ab(Equation *eqn);
void EulerImplicit(mesh *m, real *rho, real *oldphi, real dt);
void Laplacian(mesh *m, real *gamma);
real *solve(Equation *eqn, real *phi);
void volume_source(mesh *m, real *s);
void clean_matrix_in_mesh(Equation *eqn);
void print_Ab(Equation *eqn);
void arrncpy(real *to, real *from, size_t n);
void arr_setall(real *a, real value, size_t n);
void arr_fprintf(FILE *fp, const char *fmt, real *a, size_t n);


#endif 

