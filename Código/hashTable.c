#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE 20

typedef struct dataItem{
   char *key;   
   int counter;
   int firstIndex;
   int lastIndex;
   int sumIndex;
   int maxDistance;
   int minDistance;
}dataItem;

typedef struct linkedList{
	struct linkedList *prev;
	struct linkedList *next;
	struct dataItem *data;
}linkedList;

linkedList* addLinkedList(dataItem *d, linkedList *root){
  linkedList* l = malloc(sizeof(linkedList));
	printf("Adding dataItem %s at %p.\n",d->key,l);
	if(root==NULL){
		l->prev=l;
	}else{
		l->prev=root->prev;
		root->prev->next=l;
		root->prev=l;
		//l->next=NULL;
	}
	l->data=d;
	return l;
}

void showLinkedList(linkedList *link){//link=root
	while(link!=NULL){
		printf("Prev: %p\n",link->prev);
		printf("Next: %p\n",link->next);
		printf("Data: %s\n",link->data->key);
		link=link->next;
	}
}


int main() {

	dataItem *d1=malloc(sizeof(dataItem));
	d1->key="Teste1";
	linkedList *l1=addLinkedList(d1,NULL);

	printf("\nOUTPUT 1\n");
	showLinkedList(l1);

	dataItem *d2=malloc(sizeof(dataItem));
	d2->key="Teste2";
	addLinkedList(d2,l1);

	dataItem *d3=malloc(sizeof(dataItem));
	d3->key="Teste3";
	addLinkedList(d3,l1);

	dataItem *d4=malloc(sizeof(dataItem));
	d4->key="Teste3";
	addLinkedList(d4,l1);

	printf("\nOUTPUT 2\n");
	showLinkedList(l1);
}

