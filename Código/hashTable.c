#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

#define SIZE 20

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

void showDataItem(dataItem *dt)
{
	if (dt != NULL)
		printf("dataItem witk key %s, counter %d, firstIndex %d, lastIndex %d, sumIndex %d, maxDistance %d and minDistance %d.\n", dt->key, dt->counter, dt->firstIndex, dt->lastIndex, dt->sumIndex, dt->maxDistance, dt->minDistance);
	else
		printf("NULL\n");
}

//Linked list implementation
typedef struct linkedList
{
	struct linkedList *prev;
	struct linkedList *next;
	struct dataItem *data;
} linkedList;

linkedList *createLinkedList(char *key, int index)
{
	//inicialize dataItem
	dataItem *d = malloc(sizeof(dataItem));
	d->counter = 1;
	d->firstIndex = index;
	d->lastIndex = index;
	d->key = key;
	d->maxDistance = 0;
	d->minDistance = 10000000;
	d->sumIndex = index;
	//create linkedList with dataItem
	linkedList *l = malloc(sizeof(linkedList));
	l->prev = l; //when there are only one elemnent of the list, it is the first and the last (so the l->prev)
	l->data = d;
	return l;
}

void addLinkedList(char *key, int index, linkedList *root)
{
	linkedList *analising = root;

	//check for dataItem with the same key at the linked list
	bool similar = false; //true if exists dataItem with the same key, at the end of the while cycle
	while (analising->next != NULL)
	{
		if (strcmp(analising->data->key, key) == 0) //if keys are equal
		{
			similar = true;
			break;
		}
		else
		{
			analising = analising->next;
		}
	}

	//if the key already exists, update properties, otherwise, create a new node to the linkedList
	if (similar)
	{
		analising->data->counter++;						   //increment counter
		int distance = index - analising->data->lastIndex; //compute distance from last index
		//update min and max distances
		if (analising->data->minDistance > distance)
		{
			analising->data->minDistance = distance;
		}
		if (analising->data->maxDistance < distance)
		{
			analising->data->maxDistance = distance;
		}
		analising->data->lastIndex = index; //change last index
	}
	else
	{
		linkedList *l = createLinkedList(key, index);
		l->prev = root->prev; //root->prev points to the last element of the linked list (lets say last)
		root->prev->next = l; //the last->next element will be the element that is being added
		root->prev = l;		  //the element being added is the last element now, so the root->prev
							  //l->next=NULL;
	}
}

void showLinkedList(linkedList *link)
{ //link=root
	while (link != NULL)
	{
		printf("Showing %p\n", link);
		printf("Prev: %p\n", link->prev);
		printf("Next: %p\n", link->next);
		printf("Data: ");
		showDataItem(link->data);
		link = link->next;
	}
}

dataItem *getLinkedList(char *key, linkedList *link)
{
	while (link != NULL)
	{
		if (strcmp(link->data->key, key) == 0)
		{
			return link->data;
		}
	}

	return NULL;
}

//HashTable implementation
typedef struct hashTable
{
	linkedList **table; //Array
	int size;			//Array size
	int addedElements;  //Count the elements added
	int maxElements;	//Maximum element to add
} hashTable;

hashTable *createHashTable(int size)
{
	hashTable *ht = malloc(sizeof(hashTable));
	ht->table = malloc(sizeof(linkedList) * size); //Create table;
	ht->size = size;
	ht->addedElements = 0;
	int m = size * log(2);
	ht->maxElements = size * log(2); //Compute maximum number of elements that can be added
	return ht;
}

void addHashTable(char *key, int index, hashTable *hash)
{
	if (hash->maxElements == hash->addedElements)
	{
		//doubleHashTable(hash);
		printf("Doubling the size of the hashTable...\n");
	}
	else
	{
		int index = hash_function(key, hash->size); //Compute index of the key
		if (hash->table[index] == NULL)				//Create linked list
		{
			hash->table[index] = createLinkedList(key, index);
			printf("Created linkedList at index %d (root %p), that has a prev of %p with %s\n", index, &hash->table[index], hash->table[index]->prev,key);
		}
		else //Add element to linked list
		{
			addLinkedList(key, index, hash->table[index]);
			printf("Added (%s) element to linkedList at index %d\n", key, index);
		}
	}
}

//hashTable* doubleHashTable(hashTable *hash){

//}

dataItem *getHashTable(char *key, hashTable *hash)
{
	int index = hash_function(key, hash->size); //Compute index of the key

	if (hash->table[index] == NULL) //There is no linked list at this index
	{
		return NULL;
	}
	else //Get from linkedList
	{
		return getLinkedList(key, hash->table[index]);
	}
}

void showHashTable(hashTable *ht)
{
	printf("Showing hashTable stored at memory position %p...\n", ht);
	for (int i = 0; i < ht->size; i++)
	{
		printf("%d: ", i);
		if (ht->table[i] == NULL)
		{
			printf("Not used yet\n");
		}
		else
		{
			printf("LinkedList\n");
		}
	}
}

int main()
{

	//Test to LINKED LIST
	printf("Testing LINKED LIST implementation...\n");

	linkedList *l1 = createLinkedList("Primeira chave", 0);

	addLinkedList("Segunda chave", 1, l1);
	addLinkedList("Terceira chave", 2, l1);
	addLinkedList("Quarta chave", 3, l1);
	addLinkedList("Quinta chave", 4, l1);
	addLinkedList("Segunda chave", 5, l1);

	linkedList *l = l1;
	while (l != NULL)
	{
		printf("Acessing %p that has next %p and prev %p, with the dataItem that has key %s that has occured %d times (first time at %d, and last at %d)...\n", l, l->next, l->prev, l->data->key, l->data->counter, l->data->firstIndex, l->data->lastIndex);
		if (l->next != NULL)
			assert(l->next->prev == l);
		if (l->prev->next != NULL)
			assert(l->prev->next == l);
		l = l->next;
	}

	printf("All tests passed!\n");

	//Test to HASH TABLE
	printf("\nTesting HASH TABLE implementation...\n");

	hashTable *ht = createHashTable(10);

	printf("Created hash table (at memory position %p) with a length of %d that can carry up to %d elements, before being resized.\n", ht, ht->size, ht->maxElements);

	addHashTable("Primeira chave", 0, ht);
	addHashTable("Segunda chave", 1, ht);
	addHashTable("Primeira chave", 5, ht);

	showHashTable(ht);

	printf("Checking for existance of 'Primeira chave'...\n");
	showDataItem(getHashTable("Primeira chave", ht));
	printf("Checking for existance of 'Primeira chave'...\n");
	showDataItem(getHashTable("Segunda chave", ht));
	printf("Checking for existance of 'Terceira chave'...\n");
	showDataItem(getHashTable("Teceira chave", ht));
}
