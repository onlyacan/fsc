#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  double *a;
  int n = 10;	
  int i;
  
  a = malloc(sizeof(double) * n);

  for (i = 0; i < n; i ++){
      printf("%lf ", a[i]);
  }
  
  printf("caculate sizeof(a)/sizeof(double) = %lu\n", sizeof(a)/ sizeof(double));
  
  return 0;
}
