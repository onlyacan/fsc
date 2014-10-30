#include "error.h"


/* safe malloc
 * call malloc and test if pointer NULL is returned 
*/
void *smalloc(size_t size, const char *info)
{
  void *ptr;
  if ((ptr = malloc(size))!= NULL){
    return ptr;
  }
  else{
      fprintf(stderr, "**Error:%s: %s\n", "malloc() failed", info);
      exit(-1);
  }
}


FILE *sfopen(char *fname, const char *info)
{
    FILE *fp;
    
    if ((fp = fopen(fname, "r")) == NULL) {
        fprintf(stderr, "** Error: can't open \"%s\", %s\n", fname, info);
        exit(1);
    }
    else{
        return fp;
    }

}
