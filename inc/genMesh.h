#ifndef __GEN_MESH_H__
#define __GEN_MESH_H__

/*
  GEN_MESH: generic mesh format
  ALL the mesh information are stored in arrays
*/



typedef struct
{
    /* dimension variables */
    id  nnodes;     //
    id  nfaces;     // 
    id  ncells;     // 

    /* cell fields */
    real *vol;      // volume of a cell 
    id   *nnbf;     // number of neighbor faces, e.g  nnbf(I) 
    id   *nslf;     // number of slave faces for each cell 
    id   *si_fidsoc;// starting index in array fidsor, used with fidsoc
    id   *si_fidsnc;// starting index in array fidsor, used with fidsnc 


    /* face fields*/
    vec  *S;        // face vector S
    id   *fidsoc;   // face index sorted by its owner cells.  
    id   *fidsnc;   // face index sorted by its neighbor cells.  

}genMesh;



genMesh *genMesh_read(); // read genMesh from gmsh 



#endif /*__GEN_MESH_H__*/
