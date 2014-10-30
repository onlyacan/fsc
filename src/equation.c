#include "equation.h"



/* Construct the Equaiton struct based on mesh 
 * the Ax =b in struct equation
 * Inputs:
 * ======
 * 	m:   mesh pointer
 *     
 */



/*
Allocate the equation 
for scalar field, strid = 1
for 3d vector field, strid = 3
for tensor field, strid = 9
*/

Equation *equation_constr(mesh *m, const char *name, int strid)
{
    id n; // dimension of the matrix and vector x,b
    Equation *eqn;

    // some check for strid
    if(strid != 1 || strid != 3 || strid != 9){
	fprintf(stderr, "** Error: strid in equation_constr() can only be 1,3 or 9\n");
	exit(-1);
    }

    // Allocate the equation 
    eqn = (Equation*) smalloc(sizeof(Equation) * 1, "in equation_constr()");
    
    // Allocate the attributes
    eqn-> mesh = m;
    eqn-> name = strdup(name);

    n = m->nc * strid; // calc dimension of matrix 
    eqn-> A = sm_constr (n, m->nnz * strid );
    eqn-> A_lil = sm_lil_constr(n);
    eqn-> b = smalloc(sizeof(real) * n, "in equation_constr() for eqn->b");


    return eqn;
  
}








/* convert the matrix stroed in the struct cell into
 * the Ax =b in struct equation
 * Inputs:
 * ======
 * 	eqn: equation struct pointer 
 *     
 */
void update_linsys_Ab(Equation *eqn)
{
    id i;         // cell index couter
    id j;         // col index in matrix A
    int k;        // face counter in each cellp 
    real a;       // the coeff.
    cell *cs, *c; //
    face *f;
    id nc;
    id row;
    id cellid;
    mesh *m;
  
  
    printf("Call update Ax=b ...\n");
    m = eqn-> mesh;
    nc = m->nc; 
    cs = m->cs;  

    // loop each cell, put 

    for (i =0; i < nc; i++){
	row = i+1;
	c = cs + i ;
	for(k =0; k< c->nfs; k++){ // loop the nb faces for cell p 
	    j = 0;
	    sm_lil_install(eqn->A_lil, i, j, a);      
	}
    }
}// end of func


/* solve the linear system in the equation 
 * the Ax =b in struct equation
 * Inputs:
 * ======
 * 	m:   mesh pointer
 *     
 */
real *solve(Equation *eqn, real *phi)
{
    
    update_linsys_Ab(eqn);
    eqn->A = sm_lil_to_csr(eqn->A_lil);
    // sm_fprint(stdout, A); 
  

    // solve the matrix 
    eqn->iic->flag = bicgstab(eqn->A, phi, eqn->b, 
                    eqn->iic->max_it, eqn->iic->tol, &(eqn->iic->iter), &(eqn->iic->error) );

    // check the inner iteration control object
    return phi;
}





/* update the matrix in mesh struct from EulerImplicit 
 * inputs:
 * =======
 *    m: cell pointer
 *    rho: density, a pointer of real type. 
 *    oldphi: the old field, a pointer of real type, must be an array
 *    dt: current time step 
 * outputs:
 * ========
 *    void 
 * status:
 * ========
 *     pass compile
 */

void EulerImplicit(mesh *m, real *rho, real *oldphi, real dt)
{
    
    id  i;    // cell index 
    id   nc;   // number of cells
    cell *cs, *c;   // cell p ptr 

    nc = m->nc; 
    cs = m->cs;
    
    for ( i = 0; i < nc; i++){
        c = cs+i;      
        c->ap += rho[i] * c->vol / dt;
        c->q  += rho[i] * c->vol / dt * oldphi[i];
    }
    
}






/* update the matrix in mesh struct from Laplacian Term 
 * inputs:
 *    m:     cell pointer
 *    gamma: diffusivity of the field
 * outputs:
 *    NULL 
 * status:
 *     pass compile, but not finished 
 */

void Laplacian(mesh *m, real *gamma)
{
    id  i;    // cell index 
    id   nc;   // number of cells
    cell *cs, *c;   // cell p ptr 
    id len_gamma;
    

    nc = m->nc; 
    cs = m->cs;
    for ( i = 0; i < nc; i++){
	c = cs+i;      
	c->ap += 0;
	c->q  += 0;
      }        
}


/* Volumetric source term 
 * inputs:
 *    m:  cell pointer
 *    s:  volumetric source term unit = unit of dependent value / m^3 
 * outputs:
 *    NULL 
 * status:
 *     pass compile, but not finished 
 */ 

void volume_source(mesh *m, real *s)
{
    id  i;    // cell index 
    id   nc;   // number of cells
    cell *cs, *c;   // cell p ptr 

    printf("Call Volumetric Source ...\n");
    nc = m->nc; 
    cs = m->cs;  
    
    for ( i = 0; i < nc; i++){
	c = cs+i; 
	c-> q  += s[i] * c->vol ;
    }        
}



void clean_matrix_in_mesh(Equation *eqn)
{
    id i;
    int k;
    cell *c;

    for(i = 0; i < eqn->mesh->nc; i++){
        c = eqn->mesh->cs+i;
        c->ap = 0.0; 
	printf("before clean, the q is %lf\n", c->q);
	c->q = 0.0; 
        for (k = 0; k < (c->nsf + c->nnf); k ++){
            c->al[k] = 0.0;
        }
    }
} // end of func 





void arrncpy(real *to, real *from, size_t n)
{
  size_t i;
  for(i=0; i < n; i++)
    to[i] = from[i];
}

void arr_setall(real *a, real value, size_t n)
{
  size_t i;
  for(i=0; i < n; i++)
    a[i] = value;
  
}

void arr_fprintf(FILE *fp, const char *fmt, real *a, size_t n)
{
  size_t i;
  for(i=0; i < n; i++)
    fprintf(fp, fmt, a[i]);
  
  fprintf(fp, "\n");
}

