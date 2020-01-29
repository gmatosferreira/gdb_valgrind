#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//
// Custom ordered binary tree implementation
//

typedef struct node
{
  char *word;
  long count;
  struct node *left;
  struct node *right;
}
node;

static node *new_node(char *word)
{
  node *n = (node *)malloc(sizeof(node));
  n->word = strdup(word);	//pointer to a null-terminated byte string,memory is obtained dynamically using malloc
  n->count = 1;
  n->left = NULL;
  n->right = NULL;
  return n;
}

static node *add_word(node *root,char *word)
{
  if(root == NULL)
    return new_node(word);
  node *n = root;
  while(1)
  {
    int c = strcasecmp(word,n->word);
    if     (c <  0) { if(n->left  == NULL) { n->left  = new_node(word); return root; } else n = n->left;  }
    else if(c == 0) {                                       n->count++; return root;                      }
    else            { if(n->right == NULL) { n->right = new_node(word); return root; } else n = n->right; }
  }
}

static node *get_word(node *root,char *word)
{
  if(root == NULL)
    return NULL;
  node *n = root;
  while(1)
  {
    int c = strcasecmp(word,n->word);
    if     (c <  0) { if(n->left  == NULL) { return NULL; } else n = n->left;  }
    else if(c == 0) { return n;                                                }
    else            { if(n->right == NULL) { return NULL; } else n = n->right; }
  }
}

static void free_tree(node *n)
{
  if(n != NULL)
  {
    free_tree(n->left);
    free_tree(n->right);
    free(n->word);  //ALTERADO (Memory Leak): faltava libertar este ponteiro
    free(n);	   //faltava ainda libertar a memória alocada para a criação do nó
		   // Usamos a função free ja existente do C pq a alocação de memória foi feita com malloc
  }
}

//
// Ordered binary tree query (recursive) functions
//

static long count_all_words(node *n)
{
  return(n == NULL) ? 0 : count_all_words(n->left) + n->count + count_all_words(n->right);
}

static long count_different_words(node *n)
{
  return(n == NULL) ? 0 : count_different_words(n->left) + 1 + count_different_words(n->right);
}

static void list_words(node *n)
{
// ALTERADO: condição de paragem para quando o ponteiro for nulo, de modo a que não tente aceder a memória fora da reservada para o programa 
	if(n != NULL){  
    list_words(n->left);
		printf("%6ld %s\n",n->count,n->word);
    list_words(n->right);
	}
}