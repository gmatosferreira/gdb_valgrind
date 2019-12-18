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

//Functions that will be implemented for this structure
linkedList *createLinkedList(char *key, int index);
int addLinkedList(char *key, int index, linkedList **root);
void showLinkedList(linkedList *link);
dataItem *getLinkedList(char *key, linkedList *link);

linkedList *createLinkedList(char *key, int index)
{
	printf("\tcreateLinkedList (%s,%d)\n", key, index);
	//inicialize dataItem
	dataItem *d = malloc(sizeof(dataItem));
	d->counter = 1;
	d->firstIndex = index;
	d->lastIndex = index;
	d->key = key;
	d->maxDistance = 0;
	d->minDistance = 10000000;
	d->sumIndex = index;
	printf("\t\t");
	showDataItem(d);
	//create linkedList with dataItem
	linkedList *l = malloc(sizeof(linkedList));
	l->prev = l; //when there are only one elemnent of the list, it is the first and the last (so the l->prev)
	l->data = d;
	printf("\t\tcreated linkedList at %p\n", l);
	return l;
}

int addLinkedList(char *key, int index, linkedList **root)
{
	printf("\taddlinkedList (%s,%d,%p)\n", key, index, *root);
	linkedList *analising = *root;

	//check for dataItem with the same key at the linked list
	bool similar = false; //true if exists dataItem with the same key, at the end of the while cycle
	do
	{
		printf("\t\tcomparing %s with %s, obtaining %d\n", analising->data->key, key, strcasecmp(analising->data->key, key));
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

	printf("\t\tsimilar %d\n",similar);

	//if the key already exists, update properties, otherwise, create a new node to the linkedList
	if (similar)
	{
		printf("\t\tkey already exists at position %p, updating it...\n",analising);
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
			printf("\t\tthis node isn't the only one at this linkedList and is not the first one! Making it closer to the beggining...\n");
			linkedList *tempPrev=analising->prev->prev;
			if(analising->next!=NULL)
				analising->next->prev=analising->prev;
			if(analising->prev->prev->next!=NULL)
				analising->prev->prev->next=analising;
			analising->prev->next=analising->next;
			analising->prev->prev=analising;
			analising->next=analising->prev;
			analising->prev=tempPrev;
			printf("\t\treposicioned node, the current hashTable is...\n");
			if(analising->next==*root){ //If the previous node (now next) is the root
				printf("\t\t!!!analising is now root\n");
				//linkedList **rootPointer = root;
				//*rootPointer=analising;
				*root=analising;
				//root=analising;
			}
		}
		printf("\t\tUpdated at linkedlist \t");
		showDataItem(analising->data);
	}
	else
	{
		linkedList *l = createLinkedList(key, index);
		l->prev = (*root)->prev; //root->prev points to the last element of the linked list (lets say last)
		(*root)->prev->next = l; //the last->next element will be the element that is being added
		(*root)->prev = l;		  //the element being added is the last element now, so the root->prev
							  //l->next=NULL;
		printf("\t\tAdded to linkedlist \t");
		showDataItem(l->data);
	}

	return similar; //Returns 1 if key already existed and updated, and 0 if it didn't and a new node was created

}

void showLinkedList(linkedList *link)
{ //link=root
	printf("------------------------------------------\n");
	while (link != NULL)
	{
		printf("Showing %p\n", link);
		printf("Prev: %p\n", link->prev);
		printf("Next: %p\n", link->next);
		printf("Data: ");
		showDataItem(link->data);
		link = link->next;
	}
	printf("------------------------------------------\n");
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
	printf("\tcreateHashTable(%d)\n",size);
	hashTable *ht = malloc(sizeof(hashTable));
	ht->table = malloc(sizeof(linkedList*)*size); //Create table;
	ht->size = size;
	ht->addedElements = 0;
	ht->maxElements = size * log(2); //Compute maximum number of elements that can be added
	printf("\t\thashTable created at %p\n",ht);
	return ht;
}

void doubleHashTable(hashTable *hash)
{

	printf("\tdoubleHashTable(%p)\n",hash);

	printf("\t\tStarting with the folowing hashTable...\n");
	showHashTable(hash);

	//Get data from the "old" hashTable
	linkedList **tempTable=hash->table;
	int tempSize=hash->size;

	//Change data to the "new" one
	hash->table=malloc(sizeof(linkedList*)*hash->size*2);
	hash->size=hash->size*2;
	hash->maxElements = hash->size * log(2);
	hash->addedElements=0;

	printf("\t\tResizing the hashTable to a size of %d, that can store %d elements...\n",hash->size,hash->maxElements);

	printf("\t\tThe current table is at %p and the new one at %p...\n",tempTable,hash->table);
	printf("\t\tShowing the new one...\n");
	showHashTable(hash);

	for (int i = 0; i < tempSize; i++)
	{
		if (tempTable[i] != NULL) //There is a linkedList at this index
		{
			linkedList *tempLinkedList=tempTable[i];
			linkedList *nextLinkedList=NULL;
			while(tempLinkedList!=NULL){ //Add all the nodes to the new hashTable
				nextLinkedList=tempLinkedList->next; //Because addLinkListNodeToHashTable() will change 
				addLinkListNodeToHashTable(tempLinkedList,hash);
				printf("\t-back to doubleHashTable(%p)\n",hash);
				printf("\t\tnext node to be analised is %p\n",nextLinkedList);
				tempLinkedList=nextLinkedList;
			}
		}
	}

	
	printf("\tback to doubleHashTable(%p)\n",hash);
	printf("\t\tShowing hashTable inside doubleHashTable()\n");
	showHashTable(hash);
	
}

void addHashTable(char *key, int keyIndex, hashTable *hash)
{
	printf("\taddHashTable (%s,%d,%p)\n", key, keyIndex, hash);
	if (hash->maxElements == hash->addedElements) //If this elemnt will exceed max, double size
	{
		printf("\t\tDoubling the size of the hashTable...\n");
		doubleHashTable(hash);
		printf("\tback to addHashTable (%s,%d,%p)\n", key, keyIndex, hash);
		printf("\t\thashTable %p doubled to size %d\n",hash,hash->size);
	}
	int index = hash_function(key, hash->size); //Compute index of the key
	printf("\t\thashIndex %d\n",index);
	printf("\t\tThis index points to %p \n",hash->table[index]);
	if (hash->table[index] == NULL)				//Create linked list
	{
		printf("\t\tThere is no linkedList at this index yet! Creating a new one...\n");
		hash->table[index]=createLinkedList(key, keyIndex);
		linkedList *temp = hash->table[index];
		printf("\tLinkedlist %p\n",temp);
		printf("\tCreated linkedList at index %d (root %p), that has a prev of %p with %s...\n", index, hash->table[index],temp->prev , key);
		hash->addedElements++;
	}
	else //Add element to linked list
	{
		printf("\tAdded (%s) element to linkedList at index %d...\n", key, index);
		int updatedElement = addLinkedList(key, keyIndex, &hash->table[index]);
		if(!updatedElement)
			hash->addedElements++;
	}
}

void addLinkListNodeToHashTable (linkedList *ll, hashTable *hash)
{ 
	//Add dataItem to hashTable without need to check if it already exists (auxiliates doubleHashTable())

	printf("\taddLinkListNodeToHashTable(%p,%p)\n",ll,hash);
	printf("\t\tNode is next %p prev %p and ",ll->next,ll->prev);
	showDataItem(ll->data);
	int index = hash_function(ll->data->key, hash->size);
	printf("\t\tIt will be stored at index %d that has the value %p\n",index,hash->table[index]);

	if (hash->table[index] == NULL)	//There is no linkedList at this index yet
	{

		printf("\t\tThere is no linkedList at this index yet! Adding this one...\n");
		ll->next=NULL;
		ll->prev = ll; //when there are only one elemnent of the list, it is the first and the last (so the l->prev)
		hash->table[index]=ll;
	}
	else //Add node to the already existant linkedList
	{
		printf("\t\tThere is a linkedList at this index already! Adding node...\n");
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
	printf("Showing hashTable stored at memory position %p, with size %d, %d added elements out of %d (max)...\n", ht, ht->size, ht->addedElements, ht->maxElements);
	for (int i = 0; i < ht->size; i++)
	{
		printf("%d (%p): ", i, ht->table[i]);
		if (ht->table[i] == NULL)
		{
			printf("Not used yet\n");
		}
		else
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

int main()
{

	//Test to LINKED LIST
	printf("Testing LINKED LIST implementation...\n");

	linkedList *l1 = createLinkedList("Primeira chave", 0);

	addLinkedList("Segunda chave", 1, &l1);
	addLinkedList("Terceira chave", 2, &l1);
	addLinkedList("Quarta chave", 3, &l1);
	addLinkedList("Quinta chave", 4, &l1);

	addLinkedList("Segunda chave", 5, &l1);

	showLinkedList(l1);

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

	printf("Adding %s found at index %d...\n", "Primeira chave", 4);
	addHashTable("Primeira chave", 4, ht);
	printf("Adding %s found at index %d...\n", "Segunda chave", 10);
	addHashTable("Segunda chave", 10, ht);
	printf("Adding %s found at index %d...\n", "Primeira chave", 98);
	addHashTable("Primeira chave", 98, ht);

	showHashTable(ht);

	printf("Checking for existance of 'Primeira chave'...\n");
	showDataItem(getHashTable("Primeira chave", ht));
	assert(getHashTable("Primeira chave", ht)!=NULL);
	printf("Checking for existance of 'Primeira chave'...\n");
	showDataItem(getHashTable("Segunda chave", ht));
	assert(getHashTable("Segunda chave", ht)!=NULL);
	printf("Checking for existance of 'Terceira chave'...\n");
	showDataItem(getHashTable("Teceira chave", ht));
	assert(getHashTable("Teceira chave", ht)==NULL);

	printf("All tests passed!\n");

	//Test to HASHTABLE RESIZE
	printf("\nTesting hashTable RESIZE implementation...\n");

	printf("Adding 5 more elements to the hashTable (Quarta, Quinta ... Oitava Chave)...\n");
	addHashTable("Quarta chave", 14, ht);
	addHashTable("Quinta chave", 25, ht);
	addHashTable("Sexta chave", 36, ht);
	addHashTable("Sétima chave", 47, ht);
	addHashTable("Oitava chave", 58, ht);

	showHashTable(ht);

	printf("Checking if all the elements are the old elements are there and with the same values...\n");
	showDataItem(getHashTable("Primeira chave", ht));
	assert(getHashTable("Primeira chave", ht)!=NULL);
	assert(getHashTable("Primeira chave", ht)->counter==2);
	assert(getHashTable("Primeira chave", ht)->firstIndex==4);
	assert(getHashTable("Primeira chave", ht)->lastIndex==98);
	showDataItem(getHashTable("Segunda chave", ht));
	assert(getHashTable("Segunda chave", ht)!=NULL);
	assert(getHashTable("Segunda chave", ht)->counter==1);
	assert(getHashTable("Segunda chave", ht)->firstIndex==10);
	assert(getHashTable("Segunda chave", ht)->lastIndex==10);
	//showDataItem(getHashTable("Teceira chave", ht));
	assert(getHashTable("Teceira chave", ht)==NULL);

	printf("All tests passed!\n");

	//Test to same key insertion
	printf("\nTesting the insertion of the same key and the repositioning of it in the hashTable...\n");
	printf("Going to insert Sétima Chave again, that is the second node of the linked link and should become the first!\n");
	addHashTable("Sétima chave", 65, ht);
	printf("Showing hashTable now...\n");
	showHashTable(ht);
	assert(strcasecmp(ht->table[13]->data->key,"Sétima chave")==0);

	printf("All tests passed!\n");

	printf("\nThe program has finished!\n");
}
