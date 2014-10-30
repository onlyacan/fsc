#ifndef __GMSH_H__
#define __GMSH_H__

#include "type.h"
#include "main.h"
#include "genMesh.h"

/*
 * module to handling the gmsh file format
  typical gmsh "MSH ASCII file format" file for sqaure with 4cells :

  $MeshFormat
  2.2 0 8
  $EndMeshFormat
  $PhysicalNames
  5
  1 2 "left"  // physical-dimension physical-number "physical-name"
  1 3 "top"
  1 4 "right"
  1 5 "bottom"
  2 6 "Plane"
  $EndPhysicalNames
  $Nodes
  5
  1 0 0 0          // ode-number x-coord y-coord z-coord
  2 0 0.1 0
  3 0.1 0.1 0
  4 0.1 0 0
  5 0.05 0.05 0
  $EndNodes
  $Elements
  12
  1 15 2 1 1 1  // elm-number elm-type number-of-tags < tag > … node-number-list
  2 15 2 1 2 2
  3 15 2 1 3 3
  4 15 2 1 4 4
  5  1 2 2 1 1 2
  6  1 2 3 2 2 3
  7  1 2 4 3 3 4
  8  1 2 5 4 4 1
  9  2 2 6 6 1 2 5
  10 2 2 6 6 1 5 4
  11 2 2 6 6 2 3 5
  12 2 2 6 6 3 4 5
  $EndElements

tags in element (from http://geuz.org/gmsh/doc/texinfo/gmsh.html#MSH-ASCII-file-format): 

* the first tag is the number of the physical entity to which the element belongs; 
* the second is the number of the elementary geometrical entity to which the element belongs; 
* the third is the number of mesh partitions to which the element belongs, followed by the partition ids 
(negative partition ids indicate ghost cells). A zero tag is equivalent to no tag. 
Gmsh and most codes using the MSH 2 format require at least the first two tags (physical and elementary tags).
*/

enum elem_type {line_2node = 1, triangle_3node=2, point_1node = 15};

/* defint ehe 3 important regions in gmsh file*/

typedef pt node; // node has the same type as point in mesh


typedef struct
{
    int dim;
    id  number;
    char *name; 
}physicalName;

typedef struct
{
    id ielem;
    int telem;
    int ntags;
    int *tags;
    int nnds;
    id *nds;
}elem;




typedef struct 
{
    id nnodes;
    node *nodes; 

    id nphynames;
    physicalName *phynames;

    id nelems;
    id npoints;
    id nlines;
    id ntriags;

    // elements assumed to be continus, 
    // and has types of nodes, lines, triags
    elem *elems;
    elem *points; 
    elem *lines;
    elem *triags;

}gmsh_raw;

enum FaceType **facetypes; 




gmsh_raw *read_gmsh_raw(char *fpath);



#endif /*__GMSH_H__*/
