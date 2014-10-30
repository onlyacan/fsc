/*
 * A better hash table use string as key*
 *============================================*
 * from KRC page 143
 * Author: Brain W. Kernighan, Dennis M.Ritchie
 * Modified:  Can Huang
 * Date : 2014.03.14
  
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hashTable.h"
#include "error.h"


/*make the hashTable */
hashTable *hashTable_constr(size_t hashsize)
{
    hashTable *tb;
    tb = (hashTable*) smalloc(sizeof(hashTable) * 1, "in hashTable_constr() for tb");
    tb-> hashsize = hashsize;
    tb-> table = (struct nlist **) smalloc(sizeof(struct nlist*) * tb-> hashsize, "in hashTable_constr() for tb->table");
    return tb;
}




unsigned hash(char *s, size_t hashsize)
{
    unsigned hashval;
    for(hashval = 0; *s != '\0'; s ++)
        hashval = *s + 31 * hashval;

    return hashval % hashsize;
}




/*look up: look for a s in hashtab
 * input : the key
 * return: the structure pointer
 * if fail: return NULL
*/
struct nlist *lookup_(struct nlist **hashtab, char *s, size_t hashsize)
{
    struct nlist *np;
    for(np= hashtab[hash(s, hashsize)]; np != NULL; np = np-> next)
        if (strcmp(s, np-> name) == 0)
            return np;   /* found */
    
    return NULL;  /* not found */
}

char *lookup(hashTable *tb, char *s)
{
    struct nlist *np;
    if ((np = lookup_(tb->table, s, tb->hashsize)) != NULL) 
        return np-> defn;

    return NULL;
}


/* install: put (name, defn) in hashtab*/
/* install use lookup to determin whether the name being 
 * installed is already present, if so, the new definition
 * will supersede the old one. Otherweise, a new entry is created
 *
 * exeption: returns NULL if for any reason there is no room 
 * for a new entry
 */
struct nlist *install_(struct nlist **hashtab, char *name, char *defn, size_t hashsize)
{
    struct nlist *np;
    unsigned hashval;
    
    if((np = lookup_(hashtab, name, hashsize)) == NULL) { /*not found*/
        np = (struct nlist*) malloc(sizeof(*np));
        if (np == NULL || (np->name = strdup(name)) == NULL)
            return NULL;
        hashval = hash(name, hashsize);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    }
    else  /*found*/
        free((void *) np -> defn); /*free previous defn*/
    
    if ((np->defn = strdup(defn)) == NULL)
        return NULL;
    
    return np;
}

/*
  if install success, return name,
  else return NULL 
*/

char *install(hashTable *tb, char *name, char *defn)
{    
    struct nlist *np;
    if ((np = install_(tb-> table, name, defn, tb-> hashsize)) != NULL) return name;
    
    return NULL; 
    
} 


