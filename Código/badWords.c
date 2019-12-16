#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

char * badWord(int, char*);

int main(int argc, char **argv){
    char s[100]="O joao` foi as compra's, comprou um umb'igo! falante por £233";
    int n = sizeof(s);
    
    printf("%s\n", badWord(n, s));
}

char * badWord(int n, char s[n]){
    char palavra[n];
    int index=0;
    for(int i=0; i<n; i++){
        char c = s[i];
        char c2 = s[i+1];
        
        if(isalnum(c) || (ispunct(c) && isalpha(c2))){
            palavra[index]=c;
            index++;
        }
    }
    return palavra;
}
