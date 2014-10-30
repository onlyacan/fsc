#include <stdio.h>
#include "laspack/vector.h"


int main(void)
{
    Vector a;
    size_t Ind;
    size_t MaxInd;
    
    V_Constr(&a, "a", 5, Normal, True);

    for (Ind = 1; Ind <= 5; Ind ++)
        V_SetCmp(&a, Ind, (double) Ind);

    printf("get value by V_GetCmp()\n");
    for (Ind = 1; Ind <= 5; Ind ++)
        printf("%lf\n", V_GetCmp(&a, Ind));

    printf("get value by data\n");
    MaxInd = sizeof(a.Cmp);
    printf("%li Elements\n", MaxInd);
    for (Ind = 1; Ind <= 5; Ind ++)
        printf("%lf\n", a.Cmp[Ind]);
    for (Ind = 0; Ind < MaxInd; Ind ++)
        printf("%li: %lf\n", Ind, a.Cmp[Ind]);

    printf("The next question is: if we can set value by Cmp?\n");
    for (Ind = 1; Ind <= 5; Ind ++)
        a.Cmp[Ind] = (double) Ind * 10.0;

    printf("get value by V_GetCmp()\n");
    for (Ind = 1; Ind <= 5; Ind ++)
        printf("%lf\n", V_GetCmp(&a, Ind));
    printf("==>Yes\n");

    return 0;
}
