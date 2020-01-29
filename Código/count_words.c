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
#include "elapsedtime.h"

// Main program
//

int main(int argc,char **argv)
{
  // command line options
  int opt = -1;
  if(argc >= 3 && strcmp(argv[1],"-a") == 0) opt = 'a'; // all words
  if(argc >= 3 && strcmp(argv[1],"-d") == 0) opt = 'd'; // different words
  if(argc >= 3 && strcmp(argv[1],"-h") == 0) opt = 'h'; // hashTable
  if(argc >= 3 && strcmp(argv[1],"-b") == 0) opt = 'b'; // binaryTree
  if(argc >= 3 && strcmp(argv[1],"-n") == 0) opt = 'n'; // hashTable without resize
  if(argc >= 3 && strcmp(argv[1],"-t") == 0) opt = 't'; // hashTable evolution in time
  if(argc >= 3 && strcmp(argv[1],"-o") == 0) opt = 'o'; // studies occurrences of a given word
  if(opt < 0)
  {
    //fprintf(stderr,""); // blink on (may not work in some text terminals), text in red \e[5;31m
    fprintf(stderr,"usage: %s -a text_file ...  # count the number of words\n",argv[0]);
    fprintf(stderr,"       %s -d text_file ...  # count the number of different words\n",argv[0]);
	  fprintf(stderr,"       %s -h text_file ...  # shows word information from a hashTable\n",argv[0]);
	  fprintf(stderr,"       %s -b text_file ...  # shows words information from a sorted Binary Tree\n",argv[0]);
    fprintf(stderr,"       %s -n text_file ...  # creates hash table that does NOT resize and outputs it's data to the file\n",argv[0]);
    fprintf(stderr,"       %s -t text_file ...  # studies hashTable size evolution through time and outputs it to a file\n",argv[0]);
    fprintf(stderr,"       %s -o text_file ...  # studies the number of occurences of some words through the file\n",argv[0]);
    fprintf(stderr,"\e[0m"); // normal output
    return 1;
  }
  
  //Para cada ficheiro
  for(int i = 2;i < argc;i++) //ALTERADO: i tem q ser menor q nº de argumentos
  {
    hashTable *h1 = createHashTable(10);
    hashTable *htNoResize = createHashTable(100);
    //Study hashTable size through time evolution
    float time[660000];
    float size[660000];
    (void)elapsed_time();
    //Study the ocurrences of some words through the file
    int ocThe[660000];

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
        badWord(word);
        root = add_word(root,word);       
        addHashTable(word,ind,h1);
        addHashTableNoDouble(word,ind,htNoResize);
        time[i]=elapsed_time();
        size[i]=(float)h1->addedElements/h1->size;
        node *the = get_word(root,"the");
        ocThe[i]=(the==NULL) ? 0 : (int)the->count;
        //printf("%d\t%d\n",ind,ocThe[i]);
        //printf("%d\t%f\n",time[i],size[i]);
        ind++;
    }
    fclose(fp);

    // report
    switch(opt)
    {
      case 'a':;
        printf("COUNT THE NUMBER OF WORDS\n");
        long int countAllHT=countAllWordsHashTable(h1);
        long int countAllBT=count_all_words(root);
        if(ind==countAllHT && countAllHT==countAllBT)
          printf("The file %s contains %d words!\n",argv[i],ind);
        else
          printf("I'm not sure how many words the file %s contains...\nI've processed %d, but hash table registered %ld (%ld) and binary tree %ld (%ld)!\n",argv[i],ind,countAllHT,countAllHT-ind,countAllBT,countAllBT-ind);
        break;
      case 'd':; 
        printf("COUNT THE NUMBER OF DIFFERENT WORDS\n");
        long int countDifBT=count_different_words(root);
        if(countDifBT==h1->addedElements)
          printf("The file %s contains %d distinct words.\n",argv[i],h1->addedElements); 
        else
          printf("I'm not sure how many distinct words the file %s contains...\nHash table has registered %d, but binary tree %ld (%ld)!\n",argv[i],h1->addedElements,countDifBT,countDifBT-h1->addedElements); 
        break;
      case 'h': printf("SHOW HASH TABLE...\n"); showHashTable(h1); break;
      case 'b': printf("SHOW BINARY TREE...\n"); list_words(root); break;
      case 'n':;
        FILE *fht = fopen("hashtablenoresize.tsv","w");
        if(fht == NULL){fprintf(stderr,"unable to create file %s\n","hashtablenoresize.tsv");return 2;}
        fprintf(fht,"Índice\tComprimento da linked list\n");
        for(int i=0;i<htNoResize->size;i++){
          fprintf(fht,"%d\t%d\n",i,countNodes(htNoResize->table[i]));
        }
        fclose(fht);
        printf("Hash table with size 500 that does NOT resize was created and it's data was saved to file hashtablenoresize.tsv with sucess!\n");
        break;
      case 't':;
        FILE *tht = fopen("hashtabletimeevolution.tsv","w");
        fprintf(tht,"tempo\tpercentagemOcupacao\n");
        for(int i=0;i<ind;i++){
          fprintf(tht,"%f\t%f\n",time[i],size[i]);
        }
        fclose(tht);
        printf("Hash table size time evolution was recorded at hashtabletimeevolution.tsv with sucess!\n");
        break;
      case 'o':;
        printf("STUDY OF THE OCCURENCES OF SOME WORDS THROUGH THE FILE\n");
        FILE *fthe = fopen("theoccurences.tsv","w");
        if(fthe == NULL){fprintf(stderr,"unable to create file %s\n","theoccurences.tsv");return 2;}
        fprintf(fthe,"indice\tocorrencias\n");
        for(int i=0;i<ind;i++){
          fprintf(fthe,"%d\t%d\n",ind,ocThe[i]);
        }
        fclose(fthe);
        printf("The occurrence of the word 'the' through the file was registed at theoccurences.tsv with sucess!\n\nThis word has the following information at hash table:\n");
        showDataItem(getHashTable("the",h1));
        break;
    }
    // free memory
    free_tree(root);
    free_ht(h1);
    //free_ht(htNoResize);
  }
  return 0;
}
