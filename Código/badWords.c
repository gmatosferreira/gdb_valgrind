#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>

char * badWord(int, char*);
char * badWord2(int, char*, int, int);
char palavra[10000000];
int main(int argc, char **argv){
    char s[100]="O joao` foi as compra's, comprou um umb'igo! falante por £233";
    int n = sizeof(s);
    
    //char* word = badWord(n, s);
    //printf("%s\n", word);
    
    // badWord
    clock_t t;
    t = clock();
    printf("Timer starts for badWord \n");
    
    badWord2(n, s, 0, 0);
    printf("%s\n", palavra);
    
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // calculate the elapsed time
    printf("The program took %f seconds to execute\n\n", time_taken);
    
    
    // badWord2
    t = clock();
    printf("Timer starts for badWord2 \n");
    
    badWord2(n, s, 0, 0);
    printf("%s\n", palavra);
    
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC; // calculate the elapsed time
    printf("The program took %f seconds to execute\n", time_taken);
}

char * badWord(int n, char s[n]){
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

char * badWord2(int n, char s[n], int currentIndex, int currentCounter){
    if(s[currentIndex]=='\0'){
        return palavra;
    }
    char c = s[currentIndex];
    char c2 = s[currentIndex+1];
    if(isalnum(c) || (ispunct(c) && isalpha(c2))){
        palavra[currentCounter]=c;
        currentCounter++;
    }
    
    currentIndex++;
    badWord2(n, s, currentIndex, currentCounter);
    
}
