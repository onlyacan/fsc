#include "field.h"

int main(void)
{
    printf("in Main...\n");
    ndarrayField *phi;
    phi = ndarrayField_constr("name of the field__", "2 x3 x4", 100 );
    ndarrayField_fprint(stdout, phi);

    phi = ndarrayField_constr("", "3x3", 1000000);
    ndarrayField_fprint(stdout, phi);

    printf("%s\n", "some not safe case:");

    /* phi = ndarrayField_constr("", "1 2 3 4 5", 100 ); */
    /* ndarrayField_print(stdout, phi); */

    /* phi = ndarrayField_constr("", "0", 100 ); */
    /* ndarrayField_print(stdout, phi); */

    return 0; 
    
}
