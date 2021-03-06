# include "bicgstab.h"
# include "ssms.h"
# include "vector.h"
# include "error.h"


/* 
 translated from the code matlab bicgstab.m 
 from templates
implicit none
  [x, error, iter, flag] = bicgstab(A, x, b, M, max_it, tol)

 bicgstab.m solves the linear system Ax=b using the 
 BiConjugate Gradient Stabilized Method with preconditioning.

 input   A        REAL matrix
         x        REAL initial guess vector
         b        REAL right hand side vector
         M        REAL preconditioner matrix
         max_it   INTEGER maximum number of iterations
         tol      REAL error tolerance

 output  x        REAL solution vector
         error    REAL error norm
         iter     INTEGER number of iterations performed
         flag     INTEGER: 0 = solution found to tolerance
                           1 = no convergence given max_it
                          -1 = breakdown: rho = 0
                          -2 = breakdown: omega = 0
 Parameter List

*/






/*
  work: working space, an array of longer than n*8 
  n - dimension of the vector of b and x, user take charge of its correctness
  return the flag of the solver 
  output to pointer:
      - x : the ouput value 
      - error: real error norm
      - iter: actually ferformed iterations
*/

int bicgstab(sm *A, real *x, real *b,  real *work, int max_it, int *iter, real tol, real *error)
{
    int i;
    int flag;      // flag status of the solution, if it fails. see above 
    int n = A->n;  // dimension of x, b. and A. user has responsibility for right dimension 
    real rho, rho_1, alpha, omega, beta, bnrm2;
    int s_lt_tol;  // bool value if vector s < tol;
    real *r, *r_tld, *v, *p, *p_hat, *s, *s_hat, *t;
    real norm_s; 
    real resid; 

    /*
    r =     (real*) smalloc(sizeof(real) * n, "in bicgstag, for r");
    r_tld = (real*) smalloc(sizeof(real) * n, "in bicgstag, for r_tld");
    v =     (real*) smalloc(sizeof(real) * n, "in bicgstag, for v");
    p =     (real*) smalloc(sizeof(real) * n, "in bicgstag, for p");
    p_hat = (real*) smalloc(sizeof(real) * n, "in bicgstag, for p_hat");
    s =     (real*) smalloc(sizeof(real) * n, "in bicgstag, for s");
    s_hat = (real*) smalloc(sizeof(real) * n, "in bicgstag, for s_hat");
    t =     (real*) smalloc(sizeof(real) * n, "in bicgstag, for t");
    */

    r     = work + 0*n;
    r_tld = work + 1*n;
    v     = work + 2*n;
    p     = work + 3*n;
    p_hat = work + 4*n;
    s     = work + 5*n;
    s_hat = work + 6*n;
    t     = work + 7*n;


    // initialization 
    *iter = 0;
    flag = 0;

    bnrm2 = norm(n, b);
    if (bnrm2 == 0.0) bnrm2 = 1.0;

    // calculate residual r = b - Ax
    vcopy(n,r,b); r = gesmv(n, -1.0, A, x, 1.0, r); // r <-- b, //r <-- -1.0 * A x + 1.0 * b 
    *error = norm(n, r) / bnrm2; 
    if (*error < tol) return 0;  // no more iteration need 

    vcopy(n, r_tld, r); // choose an arbitrary vector such that () != 0, e.g. r_tld = r 

    rho_1 = rho = alpha = omega = 1.0;  //in  origainal file only omega = 1.0, alpha, and rho_1 not initilized.
    
    for(i=0;i<n;i++){   // initilization of v,p 
	v[i] = p[i] = 0.0;
    }


    // begin *iteration
    for (*iter = 1; *iter<= max_it; (*iter)++){
	rho = dot(n, r_tld, r);   // direction vector 
	if (rho == 0.0) break; 

	if (*iter > 1){
	    beta = (rho/rho_1) * (alpha/ omega);
	    for(i=0;i<n;i++) p[i] = r[i] + beta*( p[i] - omega*v[i] ); //p = r + beta*( p - omega*v );
	}else{
	    vcopy(n, p, r); // p = r;
	}

	vcopy(n, p_hat, p); // p_hat = M \ p; 	// get the precondition matrix M, M x = p, x = M \p
	gesmv(n, 1.0, A, p_hat, 0.0, v); // v = A*p_hat;
	alpha = rho / dot(n, r_tld, v); // alpha = rho / ( r_tld'*v );
	for(i=0;i<n;i++) s[i] = r[i] - alpha * v[i]; // s = r - alpha*v;

	norm_s = norm(n,s);
	if ( norm_s < tol ){  //early convergence check
	    axpy(n, alpha, p_hat, x); // x = x + alpha*p_hat;
	    resid = norm_s / bnrm2; 
	    break;
	} // end of if 

	vcopy(n, s_hat, s); // s_hat = M \ s;    stabilizer
	gesmv(n, 1.0, A, s_hat, 0.0, t); // t = A*s_hat;
	omega = dot(n, t, s) / dot(n,t,t); // omega = ( t'*s) / ( t'*t );

	for(i=0;i<n;i++) x[i] = x[i] + alpha * p_hat[i] + omega * s_hat[i];
	//x = x + alpha*p_hat + omega*s_hat;             % update approximation

	for(i=0;i<n;i++) r[i] = s[i] - omega*t[i]; // r = s - omega*t;
	*error = norm(n, r) / bnrm2;                //check convergence
	if ( *error <= tol ) break; 
	if ( omega == 0.0 ) break;
	rho_1 = rho;

    } // end of *iteration 



    // after *iteration check the flag 

    for(i=0, s_lt_tol = 1; i < n; i++)   // check if all fulfill s <= tol 
	if (s[i] > tol){  
	    s_lt_tol = 0;
	    break; 
	}
    

    if ( *error <= tol || s_lt_tol){  // converged
	if ( s_lt_tol)
	    *error = norm(n, s) / bnrm2;
	flag =  0.0;
    }
    
    else if ( omega == 0.0 ) //breakdown
	flag = -2;   // breakdown: omega = 0
    else if ( rho == 0.0 )
	flag = -1;
    else //                                             % no convergence
	flag = 1;
      
    return flag; 

} // end of function 
