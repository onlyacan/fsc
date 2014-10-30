/* 
read the file contents to a very large string. 
author: Can Huang
date : 24.10.2013
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_ELEMENT_LEN  1024

/*
function:
1. read all char from a input and return a char pointer value.

parameters:
1. FILE *fp: the pointer of a opened file
*/

char *file_to_string(FILE *fp)
{
    register char c;
    register size_t i;
    char *s = NULL;
    size_t maxlen ;

    s = realloc(s, sizeof(char) * (STR_ELEMENT_LEN + 1));
    maxlen = STR_ELEMENT_LEN;


    for(i = 0; (c = fgetc(fp))!= EOF; ){
        //check if there are enough space to save characters
        if (i >= maxlen){
            s = realloc(s, sizeof(char) * (maxlen += STR_ELEMENT_LEN ));
        }

        s[i++] = c;
    }
    s[i++] = '\0';

    return s;

}


/* int main(int argc, char *argv[]) */
/* { */

/*     FILE *fp; */
/*     char *s = NULL; */
/*     fp = fopen(argv[1], "r"); */
/*     s = file_to_string(fp); */
/*     fprintf(stdout, "string: \n%s", s); */
    
/*     return 0; */
/* } */
