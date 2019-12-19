//
// Tomás Oliveira e Silva, AED, November 2018
//
// This program counts the number of different words contained in a text file.
// It has a memory leak, because it does not free all the memory it allocates. SOLVED!!!
// (In this case tht is not a serious problem because when the program
// terminates all its memory is automatically freed.) Confirm that this is so
// using the valgrind program, find it, and get rid of it.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hashTable.h" //para poder usar as hash
#include "badWords.h"

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
    int c = strcmp(word,n->word);
    if     (c <  0) { if(n->left  == NULL) { n->left  = new_node(word); return root; } else n = n->left;  }
    else if(c == 0) {                                       n->count++; return root;                      }
    else            { if(n->right == NULL) { n->right = new_node(word); return root; } else n = n->right; }
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

// Main program
//

int main(int argc,char **argv)
{
  // command line options
  int opt = -1;
  if(argc >= 3 && strcmp(argv[1],"-a") == 0) opt = 'a'; // all words
  if(argc >= 3 && strcmp(argv[1],"-d") == 0) opt = 'd'; // different words
  if(argc >= 3 && strcmp(argv[1],"-l") == 0) opt = 'l'; // list words
  if(argc >= 3 && strcmp(argv[1],"-h") == 0) opt = 'h'; // hashTable
  if(opt < 0)
  {
    fprintf(stderr,"\e[5;31m"); // blink on (may not work in some text terminals), text in red
    fprintf(stderr,"usage: %s -a text_file ...  # count the number of words\n",argv[0]);
    fprintf(stderr,"       %s -d text_file ...  # count the number of different words\n",argv[0]);
    fprintf(stderr,"       %s -l text_file ...  # list all words\n",argv[0]);
    fprintf(stderr,"\e[0m"); // normal output
    return 1;
  }
  for(int i = 2;i < argc;i++) //ALTERADO: i tem q ser menor q nº de argumentos
  {
    hashTable *h1 = createHashTable(70000); //só pra 100000 consegue correr até ao fim
    // read text file
    FILE *fp = fopen(argv[i],"r");
    if(fp == NULL)
    {
      fprintf(stderr,"unable to open file %s\n",argv[i]);
      return 2;
    }
    char word[64]; // words can have at most 63 bytes
    node *root = NULL;
    int ind = 0; //indice de posicao das palavras no texto
    while(fscanf(fp,"%63s",word) == 1){
      root = add_word(root,word);       
      ind++;
      printf("%d %s\n",ind,word); // teste
      strcpy(word,badWord(sizeof(word),word));
      addHashTable(word,ind,h1);      
    }
    fclose(fp);
    // report
    switch(opt)
    {
      case 'a': printf("The file %s contains %ld words\n",argv[i],count_all_words(root)); break;
      case 'd': printf("The file %s contains %ld distinct words\n",argv[i],count_different_words(root)); break;
      case 'l': printf("Contents of the file %s:\n",argv[i]); list_words(root); break;
      case 'h': printf("\nNothing by now...\n"); break;
    }
    // free memory
    free_tree(root);
  }
  return 0;
}
