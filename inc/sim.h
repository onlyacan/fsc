#ifndef __SIM_H__
#define __SIM_H__

#include <stdio.h>
#include <mesh.h>



typedef struct
{
    char *wd; // working directory 
    char *prog; // program name 
    char *path_mesh; // working directory 
    //    char *path_config; // working directory 
    // at present, not io for configuration 
    mesh *mesh;         // mesh ptr 
}sim;


sim *sim_constr(int argc, char **argv);
void sim_fprint(FILE *fp, sim *s);




#endif /*__SIM_H__*/
