/*
 * A very simple hash table mapping 
 any key to any void pointer
 *============================================*
 * based on KRC page 143
 * Author: Brain W. Kernighan, Dennis M.Ritchie
 * Modified:  Can Huang
 * Date : 2014.03.14
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>



#define HASHSIZE 1000

struct nlist{
    struct nlist *next;
    void *key;
    void *val; 
};


static struct nlist *hashtab[HASHSIZE];  /* pointer table */


/* This hashfunction is from the Szudzik's function: */
unsigned hash(void *key)
{
    int *ip;
    ip = (int*) key; 
    //return ip[0] * ip[1] % HASHSIZE;
    return (ip[0] >= ip[1] ? ip[0] * ip[0] + ip[0] + ip[1] : ip[0] + ip[1] * ip[1])% HASHSIZE;
}


/* compare the key , if 0, key are the same */
int compare_intPair(void *tar, void *src)
{
    int *target, *source;
    target = (int*) tar;
    source = (int*) src;
    if (target[0] == source[0] && target[1] == source[1])
        return 0;
    else
        return 1;
}

/*look up: look for a s in hashtab
 * input : the key
 * return: the structure pointer
 * if fail: return NULL
*/
struct nlist *lookup(void *key, int (*comp) (void *tar, void *src))
{
    struct nlist *np;
    unsigned hashval;
    hashval = hash(key);

    for(np= hashtab[hashval]; np != NULL; np = np-> next)
        if ((*comp)(np->key, key) == 0)  /* found */
            return np;   
    
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
struct nlist *install(void *key, void *val, int (*comp) (void *tar, void *src))
{
    struct nlist *np;
    unsigned hashval;
    
    if((np = lookup(key, comp)) == NULL) { /*not found*/
        // create new key-val pair
        np = (struct nlist*) malloc(sizeof(*np));
        if (np == NULL) return NULL;
        hashval = hash(key);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
        np-> key = key; 
    }else  /*found*/
        ; 

    np->val = val; 
        
    return np;
}


double get_val(int i, int j)
{
    int ip[2];
    void  *val; 
    ip[0] = i, ip[1] = j;
    val = lookup((void*) ip, compare_intPair)->val;
    printf("%p\n", val);
    return *((double*) val); 
}



int main(void)
{
    int i,n;
    int ip[2] = {12,14};
    double *val; 
    double getval; 
    

    n = 1000; 
    val = (double *) malloc(sizeof(double ) * n);
    for(i=0;i<n;i++) *(val+i) = (double) i*i;

    for(i=0; i <n; i++){
        ip[0] = i, ip[1] = i;
        printf("hash(%i,%i) = %i\n", i, i, hash(ip));
        install((void*)ip, (void*) (val+i), compare_intPair);
        printf("install (%i, %i) : %p --> %lf\n ", ip[0], ip[1], (void *)(val+i), *(val+i));
        getval = *((double *) lookup((void*) ip, compare_intPair)->val);
        printf("lookup(%i %i): %lf\n", i,i, getval);
        if (getval != (double) i*i ) exit(-1);
        
    }


    for(i=0; i <n; i++){
        ip[0] = i, ip[1] = i;
        getval = *((double *) lookup((void*) ip, compare_intPair)->val);
        printf("lookup_again(%i %i): %lf\n", i,i, getval);
        if (getval != (double) i*i ) exit(-1);
        
    }



    return 0;


}
