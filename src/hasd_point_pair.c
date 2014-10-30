/*
 * A very simple hash table use string as key*
 *============================================*
 * from KRC page 143
 * Author: Brain W. Kernighan, Dennis M.Ritchie
 * Modified:  Can Huang
 * Date : 2014.03.14
  
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HASHSIZE 1024

struct nlist{
    struct nlist *next;

    pt    p1;  // p0--> p1, p1 is key
    pt    p0;

    face  *f;
};

static struct nlist *hashtab[HASHSIZE];  /* pointer table */

unsigned hash(pt p1)
{
    return (unsigned) (p1) % HASHSIZE;
}




/*look up: look for a s in hashtab
 * input : the key
 * return: the structure pointer
*/
struct nlist *lookup(pt p1)
{
    struct nlist *np;
    for(np= hashtab[hash(p1)]; np != NULL; np = np-> next)
        if (p1 == np-> p1) 
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
struct nlist *install(char *name, char *defn)
{
    struct nlist *np;
    unsigned hashval;
    
    if((np = lookup(name)) == NULL) { /*not found*/
        np = (struct nlist*) malloc(sizeof(*np));
        if (np == NULL || (np->name = strdup(name)) == NULL)
            return NULL;
        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    }else  /*found*/
        free((void *) np -> defn); /*free previous defn*/
    
    if ((np->defn = strdup(defn)) == NULL)
        return NULL;
    
    return np;
}





int main(void)
{
    int i;

    install("father", "This is the key to father");
    printf("%s: %s\n", "father", lookup("father")->defn);
    install("father", "This is the key to ");
    printf("%s: %s\n", "father", lookup("father")->defn);
    install("mother", "This is the key to mother");
    printf("%s: %s\n", "mother", lookup("mother")->defn);

    char key[64];
    struct nlist *np = NULL;

    for(i = 0; i < 10000; i ++){
        sprintf(key, "%i", i);
        install (key, key);
    }
    
    while (scanf("%s", key) == 1){
       ;
       printf("%s: %s\n", 
              key, ( np = lookup(key))==NULL? "Can not found": np->defn);
    }

    return 0;

}
