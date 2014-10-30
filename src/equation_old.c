#include "equation.h"



/* Construct the Equaiton struct based on mesh 
 * the Ax =b in struct equation
 * Inputs:
 * ======
 * 	m:   mesh pointer
 *     
 */




Equation *equation_constr(mesh *m, const char *name)
{
  


  id    nc; // number of cells
  cell  *cs;
  cs = m-> cs;
  nc = m-> nc;

  // Allocate the equation 
  Equation *eqn;
  eqn = (Equation*) smalloc(sizeof(Equation) * 1, "in equation_constr()");

  
  eqn-> mesh = m;
  strcpy(eqn->name, name);
  

   
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
  id i;        // cell index couter
  int k;       // face counter in each cellp 
  cell *cs, *c;  //
  face *f;
  id nc;
  id row;
  id cellid;
  mesh *m;
  
  
  printf("Call update Ax=b ...\n");
  m = eqn-> mesh;
  nc = m->nc; 
  cs = m->cs;  

  for (i =0; i < nc; i++){
    row = i+1;
    c = cs + i ;
    
    /* // set the b vector from q in a cell  */
    /* printf("set q = %lf into eqn->b\n", c->q); */
    /* V_SetCmp(eqn->b, row, c->q); */
    
    /* // set the A matrix from the ap and al[k] for faces */
    /* Q_SetEntry(eqn->A, row, 0, c->ind, c->ap); */
    
    /* for(k = 0; k < (c->nsf + c-> nnf); k ++){ */
    /*   f = c->fs[c->nbf + k]; */
    /*   if(f->on == c) // face is slave face  */
    /* 	  cellid = f->nb->ind; */
    /*   else           // face is neighbour face */
    /* 	  cellid = f->on->ind; */
    /*   Q_SetEntry(eqn->A, row, k+1, cellid, c->al[k]); */
    /* } */
    
  }
  
}


/* solve the linear system in the equation 
 * the Ax =b in struct equation
 * Inputs:
 * ======
 * 	m:   mesh pointer
 *     
 */
real *solve(Equation *eqn, real *phi)
{
    /* id i; */
    /* id nc = eqn->mesh->nc; */
    
    /* update_linsys_Ab(eqn); */
    /* SetRTCAccuracy(1e-5); */
    /* for (i=0; i < nc; i++) V_SetCmp(eqn->x, i+1, phi[i]);  */
    /* //CGIter(eqn->A, eqn->x, eqn->b, 100, SSORPrecond, 1.2);   */
    /* BiCGSTABIter(eqn->A, eqn->x, eqn->b, 100, NULL, 1.2);  */
    /* for (i=0; i < nc; i++) phi[i] = V_GetCmp(eqn->x, i+1);  */
    return phi ;
}





/* update the matrix in mesh struct from EulerImplicit 
 * inputs:
 * =======
 *    m: cell pointer
 *    rho: density, a pointer of real type. test length before usage.
 *    old: the old field, a pointer of real type, must be an array
 *    r:   rank of the rho array
 *    dt: current time step 
 * outputs:
 * ========
 *    void 
 * status:
 * ========
 *     pass compile
 */

void EulerImplicit(mesh *m, real *rho, enum Rank r, real *old, real dt)
{
    
    id  i;    // cell index 
    id   nc;   // number of cells
    cell *cs, *c;   // cell p ptr 
    
    printf("Call EulerImplict ...\n");
    nc = m->nc; 
    cs = m->cs;
    
    if (r== scalar){
      for ( i = 0; i < nc; i++){
	c = cs+i;      
	c->ap += (*rho) * c->vol / dt;
	printf("old[i]=%lf\n", old[i]);
	printf("set (*rho) * c->vol / dt * old[i] = %lf to c->q in EulerImplicit()\n", (*rho) * c->vol / dt * old[i]);
	c->q  += (*rho) * c->vol / dt * old[i];
      }      
    }
    else if (r == vector) {
      for ( i = 0; i < nc; i++){
	c = cs+i;      
	c->ap += rho[i] * c->vol / dt;
	c->q  += rho[i] * c->vol / dt * old[i];
      }        
      
    }
    else{
      fprintf(stdout, "**Error:%s\n", "For EulerImplicit(), the input rho is a bad array");
      exit(-1);
      
    }



}




/* update the matrix in mesh struct from Laplacian Term 
 * inputs:
 *    m:     cell pointer
 *    gamma: diffusivity of the field, a ptr to real scalar or array
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
    

    printf("Call Laplacian ...\n");
    len_gamma = sizeof(gamma) / sizeof(real);
    nc = m->nc; 
    cs = m->cs;
    
    if (len_gamma == 1){
      for ( i = 0; i < nc; i++){
	c = cs+i;      
	c->ap += 0;
	c->q  += 0;
      }      
    }
    else if (len_gamma == nc) {
      for ( i = 0; i < nc; i++){
	c = cs+i;      
	c->ap += 0;
	c->q  += 0;
      }        
      
    }
    else{
      fprintf(stdout, "**Error:%s\n", "For EulerImplicit(), the input rho is a bad array");
      exit(-1);
      
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

void volume_source(mesh *m, real *s, enum Rank r)
{
    id  i;    // cell index 
    id   nc;   // number of cells
    cell *cs, *c;   // cell p ptr 

    printf("Call Volumetric Source ...\n");
    nc = m->nc; 
    cs = m->cs;  
    
    if (r == scalar){
      for ( i = 0; i < nc; i++){
	c = cs+i;
	printf("going to assgin %lf to c->q = %lf\n", (*s) * c-> vol , c->q);
	c->q  += (*s) * c->vol;
	
      }      
    }
    else if (r == vector) {
      for ( i = 0; i < nc; i++){
	c = cs+i; 
	c-> q  += s[i] * c->vol ;
      }        
    }
    else{
      fprintf(stdout, "**Error:%s\n", "For volume_source(), the input s is a bad array");
      exit(-1);
      
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

}



/* // print the A, b for debug */
/* void print_Ab(Equation *eqn){ */
/*     id i,j; */
/*     id dim, dim_A; */
/*     int len; */
/*     id *js; */
/*     real *vals, val ;  */
/*     int k; */
    

/*     // print the Matrix A */
/*     dim_A = Q_GetDim(eqn->A); */

/*     js = (id *) malloc(sizeof(id) * dim_A); */
/*     vals = (real *)malloc(sizeof(real) * dim_A); */

/*     for(i = 1; i <=dim_A; i++){ */
        
/*         len = Q_GetLen(eqn->A, i); */
/*         for(j=0; j < dim_A; j ++) {js[j]=0; vals[j] = 0.0;} */
/*         for(k =0; k < len; k ++){ */
/*             j = Q_GetPos(eqn->A, i, k); */
/*             //printf("j[%i]=%lli", k, j); */
/*             val = Q_GetVal(eqn->A, i, k); */
/*             js[j-1] = j ; */
/*             vals[j-1] = val; */
/*         } */
/*         printf("\n"); */

/*         for(j = 1; j <=dim_A; j++){ */
/*             if (js[j-1] != 0) { */
/*                 printf("%6.2e ", vals[j-1]); */
/*             } */
/*             else { */
/*                 printf("%6s ", "X"); */
/*             } */
            
/*         } */
/*         printf("\n"); */
        
/*     } */

/*     printf("vector b[%lli]:\n", (dim = V_GetDim(eqn->b))); */
/*     for (i =1; i <= dim;i++){ */
/*         printf("%12.3f ", V_GetCmp(eqn->b, i)); */
/*     } */
/*     printf("\n"); */
/* } */


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
// dumped code
// add term to an equation
/* Term *add_term(Equation *eqn, Term *t) */
/* { */
/*     eqn->maxnterms = sizeof(eqn->terms) / sizeof(Term); */
/*     if (eqn->maxnterms == 0 || eqn->nterms + 1 < eqn->maxnterms){  */
/* 	// no enough space for the coming term t */
/*         eqn->terms = realloc(eqn->terms, sizeof(Term) * (eqn->maxnterms += NTERM)); */
/*     }else{ */
/* 	eqn->terms[eqn->nterms++] = t;	 */
/*     } */
/* } */

