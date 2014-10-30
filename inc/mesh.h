#ifndef __MESH_H__
#define __MESH_H__
# include <math.h>
# include "type.h"
# include "main.h"







/*
  a node is a array of real number 
*/
typedef real*  vec;  // vector 
typedef vec    pt;
typedef struct Cell cell;

/* for a cell, a face can be 
   1. slave face: face vector pointing outwards
   2. neighbour face: face vector pointing itself

   a face itself is slave face and neighbour face for two cell
   if it's located at the boundary, it's boundary face 
*/
enum FaceType{Boundary , Internal};
enum FaceRelationship{Slave, Neighbor};   

typedef struct Face 
{
    int  np;
    pt   *ps;
    cell *on;  // owner
    cell *nb;  // neigbour
    int  type; // can be any type  
    pt   ct;   // face center vector
    vec  S;    // face vector = n * area

} face;


typedef struct Cell 
{
    id  ind;     // index of cell 

    /* topology relations: faces */
    vec *ps;   // points
    vec nps;   // number of points in a cell 

    /* topology relations: faces */
    face** fs;   // all faces in one continues array 
    int nfs;     // number of total faces 
    int nbf;     // number of boundary faces
    int nsf;     // number of slave faces 
    int nnf;     // number of neighbour faces 
    int *fr;      // face relationship 

    /* some bisc values*/
    pt   ct;      // cell center 
    real vol;     // cell volume
    id   ind_elem; // id of the elem for this cell
    
    /* For the linear equation*/    
    real ap;     // coefficient in linear equation
    real *al;    // coeff for nb cell by face f, len=number of faces
    real q;      // source term, in b vector in Ax=b
}cell;




typedef struct{
    // points
    pt   *ps;
    id    np;

    // faces
    face *fs;
    id    nf;

    // cells 
    cell *cs;
    id    nc;

    // others 
    id    nbf;
    id    nnz;

} mesh;




mesh *read_gmsh(const char *fname);                        // read the gmsh output.
void fprint_mesh(FILE *fp, mesh *m);                       // print mesh info for checking
void fprint_mesh_info(FILE *fp, mesh *m);                  // print mesh info for checking





#endif 

