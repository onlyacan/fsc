#ifndef __ERROR_H__
#define __ERROR_H__

#include <stdio.h>
#include <stdlib.h>

// safe malloc 
void *smalloc(size_t size, const char *info);
FILE *sfopen(char *fname, const char *info); // open file safely

#endif 
