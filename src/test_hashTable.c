# include <stdio.h>
# include <string.h>
# include "hashTable.h"

void test_hash(hashTable *tb, char *key, char *defn)
{
    install(tb, key, defn);
    printf("test: key = \"%s\", value = \"%s\" ", key, lookup(tb, key));
    if (strcmp(defn, lookup(tb, key)) == 0)
        printf("test Pass!\n");
    else{
        printf("*** ERROR: ****test not pass \n");
        exit(-1);
    }
}


int main(void)
{
    printf("make a hash table mat\n");
    hashTable *mat = hashTable_constr(128); 
    
    printf("======= test correctness =========\n");
    test_hash(mat, "123", "one two three");



    printf("====== test exception handling ========\n");

    printf("test a blank key\n");
    test_hash(mat, "", "value for blank key");
    test_hash(mat, "key for a blank defn", "");
    printf("search a unknown key\n");
    printf("%p\n", lookup(mat, "unkown key"));



    printf("======= test performance =========\n");
    printf("install and lookup for %i times\n", 1000000);

    int i ;
    for(i=0; i < 1000000; i++){
        install(mat, "1", "one hundured and twenty three\n");
        lookup(mat, "fefe");
    }
    



    return 0;
}
