#ifndef __HASH_H__
#define __HASH_H__

struct nlist{
    struct nlist *next;
    char *name;
    char *defn;
};

unsigned hash(char *s);
struct nlist *lookup(char *s);
struct nlist *install(char *name, char *defn);


#endif /*__HASH_H__*/
