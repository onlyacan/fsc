#include <stdio.h>
#include <math.h>
#define max(a,b) ( (a>b)?a:b)
#define min(a,b) ( (a<b)?a:b)


int main(void)
{
    double a[3] ;
    double b[3] ;
    printf("Input a and b:\n");
    scanf("%lf%lf",a , a+1);
    scanf("%lf%lf",b , b+1);
    printf("a=(%lf %lf)\n", a[0], a[1]);
    printf("b=(%lf %lf)\n", b[0], b[1]);
    printf("max(a,b):%lf\n", max(a[0],a[1]));
    printf("min(a,b):%lf\n", min(a[0],a[1]));
    printf("dot(a,b):%lf\n", dot(a,b));
    normal(a,b);
    printf("a=(%lf %lf)\n", a[0], a[1]);
    printf("dot(a,b) after normal:%lf\n", dot(a,b));
    printf("len(a)::%lf\n", len(a));

    return 0;
}
