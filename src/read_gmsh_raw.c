# include "gmsh.h"
# include "error.h"
# include <stdio.h>
# include <string.h>


#define MAXLINE 1024
#define MAXWORD 128 
#define MAXTAGS 6
#define MAXNODES 126  /*maxmum node 125*/
#define MAX_NFS 64 
#define MAX_SFS 64   /* slave faces */

static gmsh_raw *read_physicalNames(FILE *fp, gmsh_raw *gmsh);
static gmsh_raw *read_nodes(FILE *fp, gmsh_raw *gmsh);   
static gmsh_raw *read_elems(FILE *fp, gmsh_raw *gmsh);

char    line[MAXLINE];
char    word[MAXLINE];
char    moduleName[] = "gmsh";

/* read_gmsh_raw: read the gmsh II file and output a pointer to mesh structure */
/*================================================================================*/
gmsh_raw *read_gmsh_raw(char *fpath)
{
    // MSH ASCII file contains 3 important regions:
    // 1. physical name
    // 2. nodes
    // 3. elements
    

    gmsh_raw *gmsh; 
    FILE    *fp;

    // open mesh file and allocate gmsh strucut
    fp = sfopen(fpath, "in genMesh_read() for fp");     
    gmsh = smalloc(sizeof(gmsh_raw)*1, "in genMesh_read, for gmsh");
    
    /*find the opening mark, and read the text blocks*/
    while(fgets(line, MAXLINE, fp) != NULL){ // read line by line 
        if (sscanf(line, "%s", word) == 1){
            if (strcmp(word, "$PhysicalNames") == 0) read_physicalNames(fp, gmsh);  
            if (strcmp(word, "$Nodes") == 0)         read_nodes(fp, gmsh);  
            if (strcmp(word, "$Elements") == 0)      read_elems(fp, gmsh);  
        }
    }

    return gmsh;
} // end of function read_gmsh_raw()



/* read_nodes: read the node reagion,*/
/*=====================================*/
gmsh_raw *read_physicalNames(FILE *fp, gmsh_raw *gmsh)
{
    char funcName[] = "read_physicalNames()";
    id      i, nnames;   
    physicalName *phynames;
    


    // read number of phynames
    while(fgets(line, MAXLINE, fp) != NULL){
        if(sscanf(line, "%lli", &nnames ) == 1) break;
    } // not so safe

    phynames = smalloc(sizeof(physicalName) * nnames, "in read_PhysicalNames, for phynames");  

    for(i=0; i< nnames && fgets(line, MAXLINE, fp) != NULL;){
        if(sscanf(line, "%i %lli \"%s\"", &(phynames[i].dim), &(phynames[i].number), phynames[i].name) == 3){
            i ++;
        }
    } /* end of point reading loop*/

    if (i != nnames) {
        fprintf(stderr, "%s:%s in %s:%s\n", 
                ERROR, funcName, moduleName, "i!= nnames");
        exit(-1);
    };
    gmsh -> nphynames = nnames;
    gmsh -> phynames= phynames; 
    return gmsh; 

} // end func read_physicalNames()


/* read_nodes: read the node reagion,*/
/*===================================*/
gmsh_raw *read_nodes(FILE *fp, gmsh_raw *gmsh)
{
    id      i, inode, nnodes;   // node index 
    char    line[MAXLINE];
    real    *xy, z;     // compoents for points coordinate, z for future
    node    *nodes;  // 
    real    *ptr;  
    char    funcName[] = "read_nodes()";

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

    if (i != nnodes) {
        fprintf(stderr, "%s:%s in %s:%s\n", 
                ERROR, funcName, moduleName, "i!= nnodes");
        exit(-1);
    };
    gmsh -> nnodes = nnodes;
    gmsh -> nodes = nodes; 
    return gmsh; 

} // end func read_nodes()



/* read_nodes: read the node reagion,*/
/*===================================*/
gmsh_raw *read_elems(FILE *fp, gmsh_raw *gmsh) 
{
    char funcName[] = "read_elems()";
    int    itag;   // tag index 
    int    ind;    // node index
    char   *wd;    // temp word for strtok
    char   delimiter[] = " ,\t";
    char   line[MAXLINE];
    elem   *elems;
    id     nelems;
    id     npoints, nlines, ntriags; //number of each type 
    id     id0_triag, id0_line, id0_point;  // starting index for element types
    int    error; 
    id     i; 

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
    

    if (i != nelems && i != npoints + nlines + ntriags) {
        fprintf(stderr, "%s:%s in %s:%s\n", 
                ERROR, funcName, moduleName, 
                "i != nelems || i != npoints + nlines + ntriags");
        exit(-1);
    };

    // sort the elements according to type: point, line, triags
    // not so safe, because it's not sorted until now. 

    id0_point = 0; 
    id0_line = id0_point + npoints;
    id0_triag = id0_line + nlines;

    gmsh -> elems = elems; gmsh -> nelems = nelems; 
    gmsh -> points = elems + id0_point; gmsh -> npoints = npoints;
    gmsh -> lines  = elems + id0_line; gmsh -> nlines = nlines;
    gmsh -> triags = elems + id0_triag; gmsh -> ntriags = ntriags; 

    return gmsh;
    
} // end of func

