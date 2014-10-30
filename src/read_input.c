#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hash.h"

#define STR_ELEMENT_LEN  1024

enum state {IN,OUT};

char *file_to_string(FILE *fp);
static void clear_comment(char *s, char with_char);
static int item_to_value(char *item, char *type, char *val_name, char **val_defn);
char *file_to_string(FILE *fp);
char *fn;


int main(int argc, char *argv[])
{
    fn = argv[1];
    FILE *fp;
    fp = fopen(fn, "r");
    char *longs ; // the file is turn into a string
    char *item ;
    int nl;

    char type[512];
    char val_name[512];
    char *val_defn;
    int count ; 

    char key[512];
    struct nlist *kd;

    longs = file_to_string(fp);
    nl = 0;
    clear_comment(longs, ' '); // all the comments region are replaced by blanks
    if ((item = strtok(longs, ";")) != NULL){
        count = item_to_value(item, type, val_name, &val_defn);
        if (count == 3){
            fprintf(stdout, ">> %12s,%12s,%s\n", type, val_name, val_defn);
            nl ++;
            install(val_name, val_defn);
        }
        //fprintf(stdout, "%i: %s\n", nl, item);
    }

    while ((item = strtok(NULL, ";"))!= NULL){
        count = item_to_value(item, type, val_name, &val_defn);
        if (count == 3){
            nl++;
            fprintf(stdout, ">> %12s,%12s,%s\n", type, val_name, val_defn);
            install(val_name, val_defn);
        //fprintf(stdout, "%i: %s\n", nl, item);
        }
    }

    printf("%s\n", "input the key name: ");
    while(scanf("%s", key) !=  EOF){
        kd = lookup(key);
        printf("key(%s) = %s\n", key, (kd == NULL)? "Not found" : kd-> defn);
        printf("%s\n", "input the key name: ");
    }
    
    return 0;
}










void clear_comment(char *item, char with_char)
{
    int i;
    int cmt1, cmt2;
    cmt1 = cmt2 = OUT;  // in comment region    
    for(i = 0; item[i] != '\0'; i++){ // loop each char critical  item[i] != '\0' not item[i+1]
       
        if(item[i] == '/' && item[i+1] == '/'){ // start with // end with \n
            cmt1 = IN;
        } else if(item[i] == '/' && item[i+1] == '*'){
            cmt2 = IN;
        } else if(cmt1 == IN && item[i] == '\n'){
            item[i] = with_char;
            cmt1 = OUT;
        } else if(cmt2 == IN && item[i] == '*' && item [i+1] == '/'){
            item[i] = with_char;
            item[i+1] = with_char;
            cmt2 = OUT;
        }

        if(cmt1 == IN || cmt2 == IN){
            item[i] = with_char;
        }

    }
}










int item_to_value(char *item, char *type, char *val_name, char **val_defn)
{

    int count = 0;
    char *c;

    count = sscanf(item, "%s", type); // if count = 0, blank line
    if(count == 1){
        c = strchr(item, '=');
        if(c == NULL) {
            fprintf(stderr, "No = found in:\n%s\n", item);
            exit(-1);
        }else{
            *(c) = ' ';
            //printf("%s\n", item);
            count = sscanf(item, "%s %s", type, val_name);
            *val_defn = ++c;
            count += 1;
        }                                        
    }
    
    switch(count){
    case 0: break; // not possible if the line is not blank
    case 1: case 2: // error
        fprintf(stderr, "Error: when reading file: %s\n in line: \n %s\n", fn, item);
        exit(-1);
        break;
    case 3:
        break;
        
    }
    return count;
}



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
