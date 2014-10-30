/*
 * A very simple hash table use any void pointer
 as key*
 *============================================*
 * from KRC page 143
 * Author: Brain W. Kernighan, Dennis M.Ritchie
 * Modified:  Can Huang
 * Date : 2014.03.14
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>



#define HASHSIZE 1000000 

struct nlist{
    struct nlist *next;
    int k1;
    int k2;
    double defn; 
};


static struct nlist *hashtab[HASHSIZE];  /* pointer table */


unsigned hash(long int a, long int b)
{
    return a*b%HASHSIZE;
    //return (a >= b ? a * a + a + b : a + b * b)% HASHSIZE;
}


/*look up: look for a s in hashtab
 * input : the key
 * return: the structure pointer
 * if fail: return NULL
*/
struct nlist *lookup(int k1, int k2)
{
    struct nlist *np;

    for(np= hashtab[hash(k1,k2)]; np != NULL; np = np-> next)
        if (k1 == np->k1 && k2 == np->k2)
            return np;   /* found */
    
    return NULL;  /* not found */
}



/* install: put (name, defn) in hashtab*/
/* install use lookup to determin whether the name being 
 * installed is already present, if so, the new definition
 * will supersede the old one. Otherweise, a new entry is created
 *
 * exeption: returns NULL if for any reason there is no room 
 * for a new entry
 */
struct nlist *install(int k1, int k2, double val)
{
    struct nlist *np;
    unsigned hashval;
    
    if((np = lookup(k1,k2)) == NULL) { /*not found*/
        np = (struct nlist*) malloc(sizeof(*np));
        if (np == NULL) return NULL;
        hashval = hash(k1,k2);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
        np->k1 = k1;
        np->k2 = k2;
    }else  /*found*/
        ; 

    np->defn = val; 
        
    return np;
}





int main(void)
{
    long int i,n;
    double val;
    int pass_test = 1;
    
    n = 1000000;

    printf("test install %li element .", n);
    for (i=0;i < n; i++) install(i, i, (double) i*i );
    printf("..done\n");

    printf("test lookup %li element .", n);
    for (i=0;i < n; i++){
        val = lookup(i, i)->defn;
        // printf("(%li,%li): %lf\n", i, i, val);
        if (val != (double) i*i) {
            pass_test = 0;
            break;
        }
    }
    printf("..done\n");

    printf("test %s\n", pass_test? "pass": "not pass");
    


    return 0;


}
