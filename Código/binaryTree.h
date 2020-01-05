#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct nodeBT {    /* data[int, int]=[hash da palavra, ocorrencias da palavra] */  int palavra; int contador;  struct nodeBT* left; 
struct nodeBT* right; struct nodeBT* parent; } nodeBT; nodeBT* searchednodeBT=NULL; void searchnodeBT(nodeBT* roott, int palavra){
if(roott != NULL){ if(roott->palavra == palavra){ searchednodeBT=roott; }else{ searchednodeBT=NULL; if(palavra <= roott->palavra){
searchnodeBT(roott->left, palavra); }else{ searchnodeBT(roott->right, palavra);}}}} int minValue(nodeBT* n){ nodeBT* current = n; 
                                                while (current->left != NULL) { 
                                                    current = current->left; } 
                                                    return(current->palavra);  }
                                        nodeBT* newnodeBT(int palavra, nodeBT* parent){
                                            nodeBT* n = (nodeBT*)malloc(sizeof(nodeBT)); 
                                                    //n->data = data; 
                                                    n->palavra = palavra;
                                                        n->contador = 1;  
                                                    n->left = NULL; 
                                                        n->right = NULL;
                                                    n->parent = parent;
                                                        return(n);}
                                                    void printTree(nodeBT* root){
                                                        if(root != NULL){
                                                    printTree(root->left);
                                             printf("%d - %d\n", root->palavra, root->contador);
                                                printTree(root->right);}}
                                                nodeBT* insert(nodeBT* nodeBT, int palavra){
                                                    if (nodeBT != NULL) { 
                                                    searchnodeBT(nodeBT, palavra);
                                                    if(searchednodeBT!=NULL){
                                        searchednodeBT->contador=searchednodeBT->contador+1;
                                                    return searchednodeBT;
                                                    }else{
                                                if (palavra <= nodeBT->palavra){  
                                                    nodeBT->left  = insert(nodeBT->left, palavra); 
                                                    nodeBT->left->parent=nodeBT;
                                                }else{
                                                    nodeBT->right = insert(nodeBT->right, palavra); 
                                                nodeBT->right->parent=nodeBT;
                                                    }
                                                    return nodeBT; 
                                                    } 
                                                    }else{
                                                return(newnodeBT(palavra, NULL)); 
                                                    }
                                                    } 

  
/*int main(){ 
    int data[2];
    nodeBT *root = newnodeBT(2345235, NULL);   
    root->left = newnodeBT(2344533, root); 
    root->right = newnodeBT(3545347, root); 
    root->right->right = newnodeBT(756745, root->left); 
    
    //a inserção é feita com (raiz, valor)
    // no caso de o valor já existir, então incrementa 1
    insert(root, 756745);
    insert(root, 435643);                                              

    printTree(root);
    return 0; 
}*/