#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "mesh.h"
#include "main.h"
#include "error.h"

#define MAXLINE 512
#define MAXWORD 512
#define MAXTAGS 6
#define MAXNODES 126  /*maxmum node 125*/
#define MAX_NFS 64 
#define MAX_SFS 64   /* slave faces */

/*Macros for vector operations*/
#define add(a,b) (a[0] += b[0], a[1] += b[1])
#define sub(a,b) (a[0] -= b[0], a[1] -= b[1])
#define subto(c, a, b) (c[0] = a[0] - b[0], c[1] = a[1] - b[1])
#define scale(a, c) (a[0] *= c, a[1] *=c )
#define cross(u,v) ( u[0]*v[1] - u[1] * v[0])
#define len(a)  (sqrt(a[0] * a[0] + a[1] * a[1]))
#define len2p(a,b)  (sqrt( (a[0] - b[0])*(a[0] - b[0])  + (a[1] - b[1])*(a[1] - b[1]) ) )// len of two points
#define normal(a, b) ( a[0] = -b[1], a[1] = b[0])


/*
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

/* 
   for each node, it have such struct note 
   which point is pointed to it.
*/

typedef struct {
    id     ps[DIM * 4]; // id of points
    face  *fs[DIM * 4];
    int count;
}pointer_pf;


typedef struct 
{
    id nnodes;
    node *nodes; 

    id nphynames;
    physicalName *nphynames;

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

    // others 
    pointer_pf *ptpfs;

}gmsh_raw;

enum FaceType **facetypes; 
// for each cell, there are n faces, there is an array *facetype for n cell, there are n array

static gmsh_raw *read_physicalNames(FILE *fp, gmsh_raw *gmsh);
static gmsh_raw *read_nodes(FILE *fp, gmsh_raw *gmsh);   
static gmsh_raw *read_elems(FILE *fp, gmsh_raw *gmsh);
static mesh *elems_to_mesh(gmsh_raw *gmsh); 

// functions as building blocks
static void constr_face_pp(cell *c, id ifc, pt p0, pt p1);
static void lines_to_boundaries();
static face *pointed(pointer_pf *ptpf, id p0);
static void add_pointed(pointer_pf *ptpf, id p0, face *f);
static int  nnz_inCell(cell *c)


/*
  read_gmsh: read the gmsh II file and output a pointer to mesh structure 
  ckecked: ok
*/
mesh *read_gmsh(const char *fname)
{

    id i; // index cell 
    id j; // index for face in all faces 
    id k; // index for faces in one cell 
    gmsh_raw *gmsh; 
    mesh *m; 

    // open mesh file and allocate gmsh strucut
    fp = sfopen(fname, "in read_gmsh() for fp");     
    gmsh = smalloc(sizeof(gmsh_raw)*1, "in read_gmsh, for gmsh");
    
    /*find the opening mark, and read the text blocks*/
    while(fgets(line, MAXLINE, fp) != NULL){ // read line by line 
        if (sscanf(line, "%s", w) == 1){
            if (strcmp(w, "$PhysicalNames") == 0) read_physicalNames(fp, gmsh);  
            if (strcmp(w, "$Nodes") == 0)         read_nodes(fp, gmsh);  
            if (strcmp(w, "$Elements") == 0)      read_elems(fp, gmsh);  
        }
    }

    /* construct mesh from elements*/
    m = elems_to_mesh(gmsh);

    /* sort the faces according to its 3 types: boundary, salve, nb*/ 
    sort_faces();

    /*clean up*/
    fclose(fp);
    free(facetypes);

    return m;
}




/* 
   read_nodes: read the node reagion,
*/
gmsh_raw *read_physicalNames(FILE *fp, gmsh_raw *gmsh)
{
    id      i, nnames;   
    char    line[MAXLINE];
    physicalName   *phynames;   

    // read number of phynames
    while(fgets(line, MAXLINE, fp) != NULL){
        if(sscanf(line, "%lli", &nnames ) == 1) break;
    } // not so safe

    phynames = smalloc(sizeof(physicalName) * nnames, "in read_PhysicalNames, for phynames");  

    for(i=0; i< nnames && fgets(line, MAXLINE, fp) != NULL;){
        if(sscanf(line, "%i %li \"%s\"", &(phynames[i].dim), &(phynames[i].number), phynames[i].name) == 3){
            i ++;
        }
    } /* end of point reading loop*/

    assert(i == nnames);
    gmsh -> nphynames = nphynames;
    gmsh -> phynames= phynames; 
    return gmsh; 

} // end func read_physicalNames()


/* 
   read_nodes: read the node reagion,
*/
gmsh_raw *read_nodes(FILE *fp, gmsh_raw *gmsh)
{
    id      i, nnodes;   // node index 
    char    line[MAXLINE];
    real    *xy, *z;     // compoents for points coordinate, z for future
    nodes   *nodes;  // 
    real    *ptr;  

    // read number of nodes
    while(fgets(line, MAXLINE, fp) != NULL){
        if(sscanf(line, "%lli", &nnodes ) == 1) break;
    } // not so safe

    xy = smalloc(sizeof(real) * DIM * nnodes, "in read_nodes, for xy"); 
    nodes = smalloc(sizeof(nodes) * nnodes, "in read_nodes, for ps");  

    for(i=0; i< nnodes && fgets(line, MAXLINE, fp) != NULL;){
        ptr = xy + DIM * i;
        if(sscanf(line, "%lli %lf %lf %lf", &inode, ptr, ptr+1, &z) == 4){
            nodes[i] = ptr;
            i ++;
        }
    } /* end of point reading loop*/

    assert(i == nnodes);
    gmsh -> nnodes = nnodes;
    gmsh -> nodes = nodes; 
    return gmsh; 

} // end func read_nodes()




void read_elems(FILE *fp, gmsh_raw *gmsh) 
{
    int    itag;   // tag index 
    int    ind;    // node index
    char   *wd;    // temp word for strtok
    char   delimiter[] = " ,\t";
    char   line[MAXLINE];
    elem   *elems;
    id     nelems;
    id     npoints, nlines, ntriags; //number of each type 
    id     id0_triag, id0_line, id0_point;  // starting index for element types


    /* find the elements number */    
    while(fgets(line, MAXLINE, fp) != NULL){
        if(sscanf(line, "%lli", &nelems) == 1) break;
    }

    elems = smalloc(sizeof(elem) * nelems, "in read_elems() for elems");

    /*read each line by strtok() for flexibility*/

    error = 0;
    ntriags = nlines = npoints = 0;     /*type conter clear zero*/

    /* read line by line, by token function for flexibility*/
    
    for(i=0; i< nelems && fgets(line, MAXLINE, fp) != NULL;){
        // read index of elem, tpye of elem, and number of tags 
        if( (wd = strtok(line, delimiter)) == NULL || sscanf(wd, "%lli", &(elems[i].ielem)) != 1) error ++;
        if( (wd = strtok(NULL, delimiter)) == NULL || sscanf(wd, "%i  ", &(elems[i].telem)) != 1) error ++;
        if( (wd = strtok(NULL, delimiter)) == NULL || sscanf(wd, "%i  ", &(elems[i].ntags)) != 1) error ++;

        // read tags
        elems[i].tags  = smalloc(sizeof(int) * elems[i].ntags, "in read_elems() for elems[i].tags");
        for(itag = 0; itag < elems[i].ntags; itag++){
            if( (wd = strtok(NULL, delimiter)) == NULL || sscanf(wd, "%i",  elems[i].tags + itag) != 1) 
                error += 1;
        }

        // read node number list
        switch(elems[i].telem){
        case point_1node   :  elems[i].nnds = 1; break;
        case line_2node    :  elems[i].nnds = 2; break;
        case triangle_3node:  elems[i].nnds = 3; break;
        default:
            printf("Error: unkown element type: %i, in line:\n%s", elems[i].telem, line);
            exit(1);
            break;
        }
        elems[i].nds = smalloc(sizeof(int) * elems[i].nnds, "in read_elems() for elems[i].nds");
        for(ind = 0; ind < elems[i].nnds; ind ++){
            if( (wd = strtok(NULL, delimiter)) == NULL || sscanf(wd, "%lli", elems[i].nds + ind) != 1) 
                error += 1;
        }
        
        // check if there is any error 
        if(error){
            fprintf(stderr, "Error occurs for line:\n%s", line);
        }else{
	    // count the elemet type
            switch(elems[i].telem){
            case point_1node:    npoints ++; break;
            case line_2node:     nlines ++; break;
            case triangle_3node: ntriags ++; break;
            } 

            i++;  // no error occurs = a elmemnt read	    
        } // end of else 

    } /* end of reading elem loop*/
    
    assert(i == nelems && i == npoints + nlines + ntriags);

    // sort the elements according to type: point, line, triags
    // not so safe, because it's not sorted until now. 

    id0_point = 0; 
    id0_line = id0_point + npoints;
    id0_triag = id0_line + nlines;

    gmsh -> elems = elems; gmsh -> nelems = nelems; 
    gmsh -> points = elems + id0_point; gmsh -> npoints = npoints;
    gmsh -> lines  = elems + id0_line; gmsh -> nlines = nlines;
    gmsh -> triags = elems + id0_triag; gmsh -> ntriags = ntriags; 
    
} // end of func






/* 
   pointed: test if point p1 associated with ptpf
   is pointed by some p0, if true, return the face
   from p0-->p1, else return NULL 
*/
face *pointed(pointer_pf *ptpf, id p0)
{
    int i;
    for(i = 0; i < ptpf->count; i ++){
        if (ptpf->ps[i] == p0)
            return ptpf->fs[i];
    }
    return NULL;
}

