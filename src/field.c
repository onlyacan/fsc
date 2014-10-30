#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "field.h"





ndarrayField *ndarrayField_constr(const char *name, const char *shape, int nelem, real init_val )
{
    char *shape_str, *token;
    const char delimiters[] = ",xX; "; // blank, comma, semi-comma, x, X
    

    ndarrayField *phi; phi = malloc(sizeof(ndarrayField));
    int i;

    // step 1: copy the name
    phi->name = strdup(name);

    // step2: parse the shape string 
    shape_str = strdup(shape);

    i = 0;  
    token = strtok(shape_str, delimiters); // first token 
    sscanf(token, "%i", phi->shape+i);
    i ++; 

    while ((token = strtok(NULL, delimiters))!= NULL) {
        sscanf(token, "%i", phi->shape+i);
        i++;
    }

    phi->rank = i; 

    // some check 
    if (phi-> rank > MAX_RANK){
        fprintf(stderr, "** Error** : ndarryField \"%s\": rank = %i > MAX_RANK = %i\n",
                phi->name, phi->rank, MAX_RANK); 
        exit(-1);
    }
    check_if_shape_valid(phi);
    

    // step 3: allocate memmery 

    phi-> stride = 1;
    for (i =0; i< phi->rank; i++){
        phi->stride *= phi-> shape[i] ;
    }
    phi-> n = nelem ;
    phi-> len = phi-> stride  * phi-> n; 

    phi-> data = malloc(sizeof(double) * phi-> len);
    

    // step 4: initialize the field 
    if (init_val == 0.0){ // check if the data == 0.0 
        if (phi-> data[0] != 0.0 || phi->data[phi->len-1] != 0.0){
            ndarrayField_assign(init_val, phi);
        }
    }
    else{
        ndarrayField_assign(init_val, phi);
    }
    return phi; 

} // end of function ndarrayField() 


/*
 * shape must be positive integer
*/

void check_if_shape_valid(ndarrayField  *phi){
    int i;
    for(i =0; i < phi-> rank; i++)
        if (phi->shape[i] <=0){ 
            fprintf(stderr, "** Error** : ndarryField \"%s\": shape[%i] = %i <= 0\n",
                    phi->name, i, phi->shape[i]); 
            exit(-1);
        }
}


void ndarrayField_fprint(FILE *fp, const ndarrayField *phi)
{
    int i;
    fprintf(fp, "--------------------------------------\n");
    fprintf(fp, "%12s \"%s\"\n", "Name:", phi->name ); 
    fprintf(fp, "%12s %i\n", "Rank:", phi->rank ); 

    fprintf(fp, "%12s ", "Shape:");
    for (i =0; i < phi-> rank; i++){
        fprintf(fp, "%i, ", phi->shape[i]);
    }
    fprintf(fp, "\n");

    fprintf(fp, "%12s %i\n", "Stride:", phi-> stride); 
    fprintf(fp, "%12s %i\n", "Nelem:", phi-> n); 
    fprintf(fp, "%12s %i\n", "Len:", phi-> len); 
    fprintf(fp, "%12s %p\n", "Data ptr:", phi-> data); 

}


void ndarrayField_assign(double val, ndarrayField *x)
{
    array_assign(x->len, val, x->data);
}






