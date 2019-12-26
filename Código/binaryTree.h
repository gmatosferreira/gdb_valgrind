#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct node { 
    // data[int, int]=[hash da palavra, ocorrencias da palavra]
    int palavra;
    int contador; 
    struct node* left; 
    struct node* right; 
    struct node* parent; 
} node; 

node* searchedNode=NULL;
void searchNode(node* roott, int palavra){
    if(roott != NULL){
        if(roott->palavra == palavra){
            searchedNode=roott;
        }else{
            searchedNode=NULL;
            if(palavra <= roott->palavra){
                searchNode(roott->left, palavra);
            }else{
                searchNode(roott->right, palavra);
            }
        }
    }
}

int minValue(node* n){ 
    node* current = n; 

    while (current->left != NULL) { 
        current = current->left; 
    } 
    return(current->palavra); 
}

node* newNode(int palavra, node* parent){
    node* n = (node*)malloc(sizeof(node)); 
    //n->data = data; 
    n->palavra = palavra;
    n->contador = 1;  
    n->left = NULL; 
    n->right = NULL;
    n->parent = parent;
    return(n); 
} 

void printTree(node* root){
    if(root != NULL){
        printTree(root->left);
        printf("%d - %d\n", root->palavra, root->contador);
        printTree(root->right);
    }
}

node* insert(node* node, int palavra){
    if (node != NULL) { 
        searchNode(node, palavra);
        if(searchedNode!=NULL){
            searchedNode->contador=searchedNode->contador+1;
            return searchedNode;
        }else{
            if (palavra <= node->palavra){  
                node->left  = insert(node->left, palavra); 
                node->left->parent=node;
            }else{
                node->right = insert(node->right, palavra); 
                node->right->parent=node;
            }
            return node; 
        } 
    }else{
        return(newNode(palavra, NULL)); 
    }
} 

  
/*int main(){ 
    int data[2];
    node *root = newNode(2345235, NULL);   
    root->left = newNode(2344533, root); 
    root->right = newNode(3545347, root); 
    root->right->right = newNode(756745, root->left); 
    
    //a inserção é feita com (raiz, valor)
    // no caso de o valor já existir, então incrementa 1
    insert(root, 756745);
    insert(root, 435643);                                              

    printTree(root);
    return 0; 
}*/