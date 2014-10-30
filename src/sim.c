#include <string.h>
#include "sim.h"
#include "error.h"
#include "mesh.h"


/*
  read the argument 
  
*/

sim *sim_constr(int argc, char **argv)
{
    //int i; 
    mesh *m; 

    // step 1: find all path 
    sim *s = smalloc(sizeof(sim) * 1, "in init_sim(), for s ");
    if (argc < 1) {
        fprintf(stderr, "**Error**: %s\n", "mesh file as argument");
        exit(-1);
    }
    s-> prog = strdup(argv[0]); 
    s-> path_mesh = strdup(argv[1]); 
    sim_fprint(stdout, s);


    // step 2: read mesh from gmsh file
    m = read_gmsh(s->path_mesh);
    fprint_mesh_info(stdout, m);
    fprint_mesh(stdout, m);
    s-> mesh = m;  

    return s;
}




void sim_fprint(FILE *fp, sim *s)
{
    fprintf(fp, "%18s %s\n", "Program:", s->prog);
    fprintf(fp, "%18s %s\n", "Mesh file:", s->path_mesh);
    // fprintf(fp, "%18s %s\n", "Configuration file:", s->path_config);
}
