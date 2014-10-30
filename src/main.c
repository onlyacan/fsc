#include "main.h"
#include "mesh.h"
#include "equation.h"
#include "error.h"
#include "sim.h"
#include "field.h"


// declearation of the variables
ndarrayField *T ;    // temperature field 
ndarrayField *rho;   // density field 
ndarrayField *diff_coef; // diffusion coefficient 
Equation *Teqn;      // temperature equation 

// time control 
real dt;  // time step

// update the temperature equation 
void Teqn_update(void);

// simulation and mesh 
sim *s;
mesh *m; 


int main(int argc, char *argv[])
{
    /*==========================
     * initilize the simulation, including
     * 1. mesh read. 
     *===========================*/
    printf("fsc ...\n");
    s = sim_constr(argc, argv);
    m = s-> mesh; 
 
    /*==========================
     * Build field and Equation 
     * ===========================*/
    T     = ndarrayField_constr("Temperature", "1", s->mesh->nc, 1000.0);
    rho   = ndarrayField_constr("Density", "1", s->mesh->nc, 1200.0);
    diff_coef = ndarrayField_constr("gamma", "1", s->mesh->nc, 0.001);

    Teqn = equation_constr(s->mesh, "Temperature equation", 1); 


    /*==========================
     * The main time advance loop 
     * ===========================*/
    Teqn_update(); 

                     

    return 0;
}



/*
  update the matrix of equaiton Teqn, and then solve it
*/
void Teqn_update(void)
{
    
    EulerImplicit(m, rho->data, T->old, dt);
    Laplacian(m, diff_coef->data);
    solve(Teqn, T->data);
}

