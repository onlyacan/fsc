#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#include <stdlib.h>

struct nlist{
    struct nlist *next;
    char *name;
    char *defn;
};



typedef struct 
{
    struct nlist* table;
    size_t hashsize; 

}hashTable;

 

hashTable *hashTable_constr(size_t hashsize);
char *lookup(hashTable *tb, char *s);
char *install(hashTable *tb, char *name, char *defn);

// unsigned hash(char *s); internal function 

#endif /*__HASH_TABLE_H__*/


