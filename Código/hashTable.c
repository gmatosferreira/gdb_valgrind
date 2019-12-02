#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define SIZE 20

// Data item implementation
typedef struct dataItem
{
	char *key;
	int counter;
	int firstIndex;
	int lastIndex;
	int sumIndex;
	int maxDistance;
	int minDistance;
} dataItem;

//Linked list implementation
typedef struct linkedList
{
	struct linkedList *prev;
	struct linkedList *next;
	struct dataItem *data;
} linkedList;

linkedList *addLinkedList(dataItem *d, linkedList *root)
{
	linkedList *l = malloc(sizeof(linkedList));
	if (root == NULL)
	{
		l->prev = l; //when there are only one elemnent of the list, it is the first and the last (so the l->prev)
	}
	else
	{
		l->prev = root->prev; //root->prev points to the last element of the linked list (lets say last)
		root->prev->next = l; //the last->next element will be the element that is being added
		root->prev = l;		  //the element being added is the last element now, so the root->prev
							  //l->next=NULL;
	}
	l->data = d;
	return l;
}

void showLinkedList(linkedList *link)
{ //link=root
	while (link != NULL)
	{
		printf("Showing %p\n", link);
		printf("Prev: %p\n", link->prev);
		printf("Next: %p\n", link->next);
		printf("Data: %s\n", link->data->key);
		link = link->next;
	}
}

//HashTable implementation
typedef struct hashTable
{
	struct linkedList; //Array
	int addedElements; //Count the elements added
} hashTable;

void *addHashTable(char *key, hashTable *hash)
{
	//TODO
}

unsigned int hash_function(const char *str, unsigned int s)
{ //returns the hashcode of *str between 0 and s
	static unsigned int table[256];
	unsigned int crc, i, j;
	if (table[1] == 0u) // do we need to initialize the table[] array?
		for (i = 0u; i < 256u; i++)
			for (table[i] = i, j = 0u; j < 8u; j++)
				if (table[i] & 1u)
					table[i] = (table[i] >> 1) ^ 0xAED0AED0AED0011Full; // "magic" constant
				else
					table[i] >>= 1;
	crc = 0xAED02019u; // initial value (chosen arbitrarily)
	while (*str != '\0')
		crc = (crc >> 8) ^ table[crc & 0xFFu] ^ ((unsigned int)*str++ << 24);
	return crc % s;
}

int main()
{

	printf("Testing linked list implementation...\n");

	dataItem *d1 = malloc(sizeof(dataItem));
	d1->key = "Teste1";
	linkedList *l1 = addLinkedList(d1, NULL);

	dataItem *d2 = malloc(sizeof(dataItem));
	d2->key = "Teste2";
	addLinkedList(d2, l1);

	dataItem *d3 = malloc(sizeof(dataItem));
	d3->key = "Teste3";
	addLinkedList(d3, l1);

	dataItem *d4 = malloc(sizeof(dataItem));
	d4->key = "Teste4";
	addLinkedList(d4, l1);

	linkedList *l = l1;
	while (l != NULL)
	{
		if (l->next != NULL)
			assert(l->next->prev == l);
		if (l->prev->next != NULL)
			assert(l->prev->next == l);
		l = l->next;
	}
	printf("All tests passed!\n");
}
