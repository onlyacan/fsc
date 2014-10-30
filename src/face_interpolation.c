#include <stdio.h>

#include "mesh.h"
#include "field.h"

/*purpose: calculate the interpolation factor using CDS
* use the equation from Rusche PHD Thesis 2002.
* becuase it's more clearer than the Peric and Jasak
* $
 f_x = \frac{|\vec{x}_f - \vec{x}_N|}{|\vec{x}_f - \vec{x}_N| + |\vec{x}_f - \vec{x}_P|}
$
*/
void calc_cds_fx(mesh *m, real *fxs)
{
    id nf = m-> nf;
    face *fs = m-> fs;
    face *f;

    id i;
    for(i=0; i< nf; i++){
        f = fs+i;
	if(f-> internal == TRUE)
	    fxs[i] = len2p(f->ct, f->nb->ct)/(len2p(f->ct, f->nb->ct) + len2p(f->ct, f->on->ct));
	else
	    fxs[i] = 0.0;
    }
}
