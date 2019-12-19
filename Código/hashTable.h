#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

unsigned int hash_function(const char *str, unsigned int s)
{ //returns the hashcode of *str between 0 and s
	static unsigned int table[256];
	unsigned int crc, i, j;
	if (table[1] == 0u){ // do we need to initialize the table[] array?
		for (i = 0u; i < 256u; i++){
			for (table[i] = i, j = 0u; j < 8u; j++){
				if (table[i] & 1u)
					table[i] = (table[i] >> 1) ^ 0xAED0AED0AED0011Full; // "magic" constant
				else
					table[i] >>= 1;
			}
		}
	}
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
	int maxDistance;
	int minDistance;
} dataItem;

void showDataItem(dataItem *dt)
{
	if (dt != NULL)
		printf("dataItem witk key %s, counter %d, firstIndex %d, lastIndex %d, maxDistance %d and minDistance %d.\n", dt->key, dt->counter, dt->firstIndex, dt->lastIndex, dt->maxDistance, dt->minDistance);
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

//Functions that will be implemented for this structure
linkedList *createLinkedList(char *key, int index);
int addLinkedList(char *key, int index, linkedList **root);
void showLinkedList(linkedList *link);
dataItem *getLinkedList(char *key, linkedList *link);

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
	//create linkedList with dataItem
	linkedList *l = malloc(sizeof(linkedList));
	l->prev = l; //when there are only one elemnent of the list, it is the first and the last (so the l->prev)
	l->data = d;
	return l;
}

int addLinkedList(char *key, int index, linkedList **root)
{
	linkedList *analising = *root;

	//check for dataItem with the same key at the linked list
	bool similar = false; //true if exists dataItem with the same key, at the end of the while cycle
	do
	{
		if (strcasecmp(analising->data->key, key) == 0) //if keys are equal
		{
			similar = true;
			break;
		}
		else
		{
			analising = analising->next;
		}

	} while (analising != NULL);

	//if the key already exists, update properties, otherwise, create a new node to the linkedList
	if (similar)
	{
		analising->data->counter++;						   //increment counter
		int distance = index - analising->data->lastIndex; //compute distance from last index
		//update min and max distances
		if (analising->data->minDistance > distance)
			analising->data->minDistance = distance;
		if (analising->data->maxDistance < distance)
			analising->data->maxDistance = distance;
		if(analising->data->lastIndex<index)
			analising->data->lastIndex = index; //change last index
		if(analising->data->firstIndex>index)
			analising->data->firstIndex = index; //change first index
		//make node closer to the beggining of the linkedList (one node closer)
		if(analising->prev!=analising){ //If analising isn't the only the node of the linkedList
			linkedList *tempPrev=analising->prev->prev;
			if(analising->next!=NULL)
				analising->next->prev=analising->prev;
			if(analising->prev->prev->next!=NULL)
				analising->prev->prev->next=analising;
			analising->prev->next=analising->next;
			analising->prev->prev=analising;
			analising->next=analising->prev;
			analising->prev=tempPrev;
			if(analising->next==*root){ //If the previous node (now next) is the root
				*root=analising;
			}
		}
	}
	else
	{
		linkedList *l = createLinkedList(key, index);
		l->prev = (*root)->prev; //root->prev points to the last element of the linked list (lets say last)
		(*root)->prev->next = l; //the last->next element will be the element that is being added
		(*root)->prev = l;		  //the element being added is the last element now, so the root->prev
							  //l->next=NULL;
	}

	return similar; //Returns 1 if key already existed and updated, and 0 if it didn't and a new node was created

}

void showLinkedList(linkedList *link)
{ //link=root
	while (link != NULL)
	{
		printf("Showing %p\n", link);
		printf("Prev: %p\n", link->prev);
		printf("Next: %p\n", link->next);
		printf("Data: ");
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

//Functions that will be implemented for this structure
hashTable *createHashTable(int size);
void doubleHashTable(hashTable *hash);
void addHashTable(char *key, int keyIndex, hashTable *hash);
void addLinkListNodeToHashTable (linkedList *ll, hashTable *hash);
dataItem *getHashTable(char *key, hashTable *hash);
void showHashTable(hashTable *ht);


hashTable *createHashTable(int size)
{
	hashTable *ht = malloc(sizeof(hashTable));
	ht->table = malloc(sizeof(linkedList*)*size); //Create table;
	ht->size = size;
	ht->addedElements = 0;
	ht->maxElements = size * log(2); //Compute maximum number of elements that can be added
	return ht;
}

void doubleHashTable(hashTable *hash)
{
	//Get data from the "old" hashTable
	linkedList **tempTable=hash->table;
	int tempSize=hash->size;

	//Change data to the "new" one
	hash->table=malloc(sizeof(linkedList*)*hash->size*2);
	hash->size=hash->size*2;
	hash->maxElements = hash->size * log(2);
	hash->addedElements=0;

	for (int i = 0; i < tempSize; i++)
	{
		if (tempTable[i] != NULL) //There is a linkedList at this index
		{
			linkedList *tempLinkedList=tempTable[i];
			linkedList *nextLinkedList=NULL;
			while(tempLinkedList!=NULL){ //Add all the nodes to the new hashTable
				nextLinkedList=tempLinkedList->next; //Because addLinkListNodeToHashTable() will change 
				addLinkListNodeToHashTable(tempLinkedList,hash);
				tempLinkedList=nextLinkedList;
			}
		}
	}
	
}

void addHashTable(char *key, int keyIndex, hashTable *hash)
{
	if (hash->maxElements == hash->addedElements) //If this elemnt will exceed max, double size
	{
		doubleHashTable(hash);
	}
	int index = hash_function(key, hash->size); //Compute index of the key
	if (hash->table[index] == NULL)				//Create linked list
	{
		hash->table[index]=createLinkedList(key, keyIndex);
		hash->addedElements++;
	}
	else //Add element to linked list
	{
		int updatedElement = addLinkedList(key, keyIndex, &hash->table[index]);
		if(!updatedElement)
			hash->addedElements++;
	}
}

void addLinkListNodeToHashTable (linkedList *ll, hashTable *hash)
{ 
	//Add dataItem to hashTable without need to check if it already exists (auxiliates doubleHashTable())

	int index = hash_function(ll->data->key, hash->size);

	if (hash->table[index] == NULL)	//There is no linkedList at this index yet
	{
		ll->next=NULL;
		ll->prev = ll; //when there are only one elemnent of the list, it is the first and the last (so the l->prev)
		hash->table[index]=ll;
	}
	else //Add node to the already existant linkedList
	{
		linkedList *root=hash->table[index];
		ll->prev = root->prev;
		ll->next=NULL;
		root->prev->next = ll;
		root->prev = ll;
	}
	hash->addedElements++;

}

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
	for (int i = 0; i < ht->size; i++)
	{
		if (ht->table[i] != NULL)
		{
			printf("LinkedList with: ");
			linkedList *tempLL=ht->table[i];
			while(tempLL!=NULL) {
				printf("%s, ",tempLL->data->key);
				tempLL=tempLL->next;
			}
			printf("\n");
		}
	}
}

/* int main()
{

	//Test to LINKED LIST
	printf("Testing LINKED LIST implementation...\n");

	linkedList *l1 = createLinkedList("Primeira chave", 0);

	addLinkedList("Segunda chave", 1, &l1);
	addLinkedList("Terceira chave", 2, &l1);
	addLinkedList("Quarta chave", 3, &l1);
	addLinkedList("Quinta chave", 4, &l1);

	addLinkedList("Segunda chave", 5, &l1);

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

	//Test to HASH TABLE
	printf("\nTesting HASH TABLE implementation...\n");

	hashTable *ht = createHashTable(10);

	addHashTable("Primeira chave", 4, ht);
	addHashTable("Segunda chave", 10, ht);
	addHashTable("Primeira chave", 98, ht);

	assert(getHashTable("Primeira chave", ht)!=NULL);
	assert(getHashTable("Segunda chave", ht)!=NULL);
	assert(getHashTable("Teceira chave", ht)==NULL);

	printf("All tests passed!\n");

	//Test to HASHTABLE RESIZE
	printf("\nTesting hashTable RESIZE implementation...\n");

	addHashTable("Quarta chave", 14, ht);
	addHashTable("Quinta chave", 25, ht);
	addHashTable("Sexta chave", 36, ht);
	addHashTable("Sétima chave", 47, ht);
	addHashTable("Oitava chave", 58, ht);

	assert(getHashTable("Primeira chave", ht)!=NULL);
	assert(getHashTable("Primeira chave", ht)->counter==2);
	assert(getHashTable("Primeira chave", ht)->firstIndex==4);
	assert(getHashTable("Primeira chave", ht)->lastIndex==98);
	assert(getHashTable("Segunda chave", ht)!=NULL);
	assert(getHashTable("Segunda chave", ht)->counter==1);
	assert(getHashTable("Segunda chave", ht)->firstIndex==10);
	assert(getHashTable("Segunda chave", ht)->lastIndex==10);
	assert(getHashTable("Teceira chave", ht)==NULL);

	printf("All tests passed!\n");

	//Test to same key insertion
	printf("\nTesting the insertion of the same key and the repositioning of it in the hashTable...\n");
	addHashTable("Sétima chave", 65, ht);
	assert(strcasecmp(ht->table[13]->data->key,"Sétima chave")==0);

	printf("All tests passed!\n");

	printf("\nThe program has finished!\n");
} */
