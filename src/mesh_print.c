#include <stdio.h>

#include "main.h"
#include "mesh.h"

void fprint_mesh(FILE *fp, mesh *m)
{
    /* print points */
    id i;
    pt *ps = m-> ps;
    id np = m-> np;

    face *fs = m-> fs;
    id nf = m-> nf;

    cell *cs = m-> cs;
    id nc = m-> nc;

    char dl[] = "-----------";
    fprintf(stdout, "point:\n");
    fprintf(fp, "#%11s%12s%12s%12s\n", dl, dl, dl, dl);
    fprintf(fp, "#%11s%12s%12s%12s\n", "i", "nodeID", "x", "y");
    fprintf(fp, "#%11s%12s%12s%12s\n", dl, dl, dl, dl);

    for(i=0; i < np; i++){
        fprintf(fp, "%12lli%12lli%12lf%12lf\n", i, i+1, ps[i][0], ps[i][1]);
    }

    /* print faces */
    fprintf(stdout, "face:\n");
    fprintf(fp, "#%11s%12s%12s%12s%12s%12s%12s%12s%12s%12s\n", dl, dl, dl, dl, dl, dl, dl, dl,dl, dl);
    fprintf(fp, "#%11s%12s%12s%12s%12s%12s%12s%12s%12s%12s\n", 
            "i", "nodeID", "np", "ct[0]", "ct[1]", "type", "S[0]", "S[1]", "onwer ptr", "neighbr ptr");
    fprintf(fp, "#%11s%12s%12s%12s%12s%12s%12s%12s%12s%12s\n", dl, dl, dl, dl, dl, dl, dl , dl, dl, dl);

    for(i=0; i < nf; i++){
        fprintf(fp, "%12lli%12lli%12i%12lf%12lf%12s%12lf%12lf%12p%12p\n", 
                i, i+1, fs[i].np, fs[i].ct[0], fs[i].ct[1], fs[i].internal == TRUE? "internal": "boundary", fs[i].S[0], fs[i].S[1], fs[i].on, fs[i].nb);
    }


    /* print cell */
    fprintf(stdout, "cell:\n");
    fprintf(fp, "#%11s%12s%12s%12s%12s\n", dl, dl, dl,  dl, dl);
    fprintf(fp, "#%11s%12s%12s%12s%12s\n", "i", "ElemID", "ct[0]", "ct[1]", "vol");
    fprintf(fp, "#%11s%12s%12s%12s%12s\n", dl, dl, dl, dl, dl);

    for(i=0; i < nc; i++){
        fprintf(fp, "%12lli%12lli%12lf%12lf%12lf\n", 
                i, cs[i].ind_elem, cs[i].ct[0], cs[i].ct[1], cs[i].vol);
    }
}


void fprint_mesh_info(FILE *fp, mesh *m)
{

    fprintf(stdout, ">>> mesh statistics:\n");
    fprintf(stdout, "%12s%12lli\n", "nodes: ", m->np);
    fprintf(stdout, "%12s%12lli\n", "faces: ", m->nf);
    fprintf(stdout, "%12s%12lli\n", "bfaces: ", m->nbf);
    fprintf(stdout, "%12s%12lli\n", "cells: ", m->nc);

}
