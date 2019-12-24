#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// de alguma forma à qual ainda não cheguei, temos de conseguir ordenar a árvore por hashcodes
typedef struct node { 
    //a ideia era aqui adicionar mais um campo para o número de aparições ou pôr a variável 'data' como sendo um vetor de duas posições (aparições, palavra)
    int data; 
    struct node* left; 
    struct node* right; 
    struct node* parent; 
} node; 

// a ideia é usar um vetor data[] (com tamanho 2: palavra e aparições) e, assim, procuramos por uma palavra e ele dá-nos o número de aparições da palavra.  
node* searchedNode=NULL;
void searchNode(node* roott, int data){
    if(roott != NULL){
        if(roott->data == data){
            searchedNode=roott;
        }else{
            //o codigo abaixo, aumenta a velocidade de pesquisa, ignorando logo os valores maiores/menores dependendo do que se está a procura
            if(data <= roott->data){
                searchNode(roott->left, data);
            }else{
                searchNode(roott->right, data);
            }
        }
    }
}

int minValue(node* n){ 
    node* current = n; 

    while (current->left != NULL) { 
        current = current->left; 
    } 
    return(current->data); 
}

node* newNode(int data, node* parent){  
    node* n = (node*)malloc(sizeof(node)); 

    n->data = data; 
    n->left = NULL; 
    n->right = NULL;
    n->parent = parent;
    return(n); 
} 

node* insert(node* node, int data){
    if (node == NULL)  
        return(newNode(data, NULL));   
    else { 
        searchNode(node, data);
        if(searchedNode!=NULL){
            searchedNode->data=searchedNode->data+1;
            return searchedNode;
        }else{
            if (data <= node->data){  
                node->left  = insert(node->left, data); 
                node->left->parent=node;
            }else{
                node->right = insert(node->right, data); 
                node->right->parent=node;
            }
            return node; 
        } 
    } 
} 

int printTree(node* root){
    if(root != NULL){
        printTree(root->left);
        printf("%d \n", root->data);
        printTree(root->right);
    }
    return 0;
}
  
/*int main(){ 
    node *root = newNode(1, NULL);   

    root->left        = newNode(2, root); 
    root->right       = newNode(3, root); 
    root->left->left  = newNode(4, root->left); 

    //a inserção é feita com (raiz, valor)
    // no caso de o valor já existir, então incrementa 1
    insert(root, 5); 
    insert(root, 6); 
    insert(root, 7);                                                    
    insert(root, 8);                                                       
    insert(root, 9); 
    insert(root, 9);                                                    

    printTree(root);

    printf("data do root %d\n",root->data);
    searchNode(root, 5);
    
    printf("search->right->data encontrada do searchNode(root, 5) %d\n", searchedNode->data);
    return 0; 
}*/
