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
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <time.h>

#include "badWords.h"
#include "hashTable.h"
#include "binaryTree.h"

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
  if(argc >= 3 && strcmp(argv[1],"-b") == 0) opt = 'b'; // binaryTree
  if(opt < 0)
  {
    fprintf(stderr,"\e[5;31m"); // blink on (may not work in some text terminals), text in red
    fprintf(stderr,"usage: %s -a text_file ...  # count the number of words\n",argv[0]);
    fprintf(stderr,"       %s -d text_file ...  # count the number of different words\n",argv[0]);
    fprintf(stderr,"       %s -l text_file ...  # list all words\n",argv[0]);
	  fprintf(stderr,"usage: %s -h text_file ...  # shows word information from a hashTable",argv[0]);
	  fprintf(stderr,"usage: %s -b text_file ...  # shows words information from a sorted Binary Tree\n",argv[0]);
    fprintf(stderr,"\e[0m"); // normal output
    return 1;
  }
  hashTable *h1 = createHashTable(100000);
  for(int i = 2;i < argc;i++) //ALTERADO: i tem q ser menor q nº de argumentos
  {
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
        //printf("%s\t\t",word);
        badWord(word);
        root = add_word(root,word);       
        ind++;
        addHashTable(word,ind,h1,count_different_words(root));
    }
    fclose(fp);

    // report
    switch(opt)
    {
      case 'a': printf("The file %s contains %d words according to the counter.\nHash Table counts %ld.\nBinary Tree counts %ld.\n",argv[i],ind,countAllWordsHashTable(h1),count_all_words(root)); break;
      case 'd': printf("The file %s contains %d distinct words according to hash table and %ld according to binary tree.\n",argv[i],h1->addedElements,count_different_words(root)); break;
      case 'l': printf("Contents of the file %s:\n",argv[i]); list_words(root); break;
      case 'h': printf("\nHASH TABLE with linked list...\n"); showHashTable(h1); break;
      case 'b': 
        printf("\nHASH TABLE with binary tree...\n"); 
        list_words(root);
        break;
    }
    // free memory
    free_tree(root);
  }
  return 0;
}
