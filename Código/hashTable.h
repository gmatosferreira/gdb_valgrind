

//Retirada dos slides teóricos
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

//djb2
/*unsigned int hash_function(const char *str, unsigned int s)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c;

    return hash%s;
}*/

//sdbm
/*unsigned int hash_function(const char *str, unsigned int s)
{
    unsigned long hash = 0;
    int c;

    while (c = *str++)
        hash = c + (hash << 6) + (hash << 16) - hash;

    return hash%s;
}*/

//lose lose
/*unsigned int hash_function(const char *str, unsigned int s)
{
	unsigned int hash = 0;
	int c;

	while (c = *str++)
		hash += c;

	return hash%s;
}*/


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

//Functions that will be implemented for this structure
static void showDataItem(dataItem *dt);
static void free_di(dataItem *dt);

static void showDataItem(dataItem *dt)
{
	if (dt != NULL)
		printf("dataItem witk key %s, counter %d, firstIndex %d, lastIndex %d, sumIndex %d, maxDistance %d and minDistance %d.\n", dt->key, dt->counter, dt->firstIndex, dt->lastIndex, dt->sumIndex, dt->maxDistance, dt->minDistance);
	else
		printf("NULL\n");
}

static void free_di(dataItem *dt){
	if(dt!=NULL){
		free(dt->key);
		free(dt);
	}
}

//Linked list implementation
typedef struct linkedList
{
	struct linkedList *prev;
	struct linkedList *next;
	struct dataItem *data;
} linkedList;

//Functions that will be implemented for this structure
static linkedList *createLinkedList(char *key, int index);
static int addLinkedList(char *key, int index, linkedList **root);
static void showLinkedList(linkedList *link);
static dataItem *getLinkedList(char *key, linkedList *link);
static long countAllWordsLLNode(linkedList *link);
static void free_ll(linkedList *ll);

static linkedList *createLinkedList(char *key, int index)
{
	//inicialize dataItem
	dataItem *d = malloc(sizeof(dataItem));
	d->counter = 1;
	d->firstIndex = index;
	d->lastIndex = index;
	d->key = strdup(key);
	d->maxDistance = 0;
	d->minDistance = 10000000;
	d->sumIndex = index;
	//create linkedList with dataItem
	linkedList *l = malloc(sizeof(linkedList));
	l->prev = l; //when there are only one elemnent of the list, it is the first and the last (so the l->prev)
	l->data = d;
	return l;
}

static linkedList* checkSimilar(char *key, linkedList *ll){
	if(ll==NULL) return ll;
	else return (strcasecmp(key,ll->data->key)==0) ? ll : checkSimilar(key,ll->next);
}

static int addLinkedList(char *key, int index, linkedList **root)
{
	linkedList *analising = checkSimilar(key,*root);

	//if the key already exists, update properties, otherwise, create a new node to the linkedList
	if (analising!=NULL)
	{
		analising->data->counter+=1;						   //increment counter
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
		if(analising->prev!=analising && analising!=*root){ //If analising isn't the only the node of the linkedList, nor the root
			linkedList *tempPrev=analising->prev->prev;
			if(analising->next!=NULL)
				analising->next->prev=analising->prev;
			else
				(*root)->prev=analising->prev;
			if(analising->prev->prev->next!=NULL)
				analising->prev->prev->next=analising;
			analising->prev->next=analising->next;
			analising->prev->prev=analising;
			analising->next=analising->prev;
			analising->prev=tempPrev;
			if(analising->next==*root) //If the previous node (now next) is the root
				*root=analising;
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

	return analising!=NULL; //Returns 1 if key already existed and updated, and 0 if it didn't and a new node was created

}

static void showLinkedList(linkedList *link)
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

static dataItem *getLinkedList(char *key, linkedList *link)
{
	while (link != NULL)
	{
		if (strcasecmp(link->data->key, key) == 0)
		{
			return link->data;
		}
	}

	return NULL;
}

static long countAllWordsLLNode(linkedList *link){
	return(link==NULL)? 0: (long)link->data->counter + countAllWordsLLNode(link->next);
}

static int countNodes(linkedList *link){
	return(link==NULL)? 0 : 1 + countNodes(link->next);
}

static void free_ll(linkedList *ll){
	if(ll!=NULL){
		free_ll(ll->next);
		free_di(ll->data);
		free(ll);
	}
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
static hashTable *createHashTable(int size);
static void doubleHashTable(hashTable *hash);
static void addHashTable(char *key, int keyIndex, hashTable *hash);
static void addHashTableNoDouble(char *key, int keyIndex, hashTable *hash);
static void addLinkListNodeToHashTable (linkedList *ll, hashTable *hash);
static dataItem *getHashTable(char *key, hashTable *hash);
static void showHashTable(hashTable *ht);
static long countAllWordsHashTable(hashTable *ht);
static void free_ht(hashTable *ht);


static hashTable *createHashTable(int size)
{
	hashTable *ht = malloc(sizeof(hashTable));
	ht->table = malloc(sizeof(linkedList*)*size); //Create table;
	ht->size = size;
	ht->addedElements = 0;
	ht->maxElements = size * log(2); //Compute maximum number of elements that can be added
	return ht;
}

static void doubleHashTable(hashTable *hash)
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

static void addHashTable(char *key, int keyIndex, hashTable *hash)
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

static void addHashTableNoDouble(char *key, int keyIndex, hashTable *hash)
{
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

static void addLinkListNodeToHashTable (linkedList *ll, hashTable *hash)
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

static dataItem *getHashTable(char *key, hashTable *hash)
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

static void showHashTable(hashTable *ht)
{
	for (int i = 0; i < ht->size; i++)
	{
        printf("%d (%14p)\t",i,ht->table[i]);
		if (ht->table[i] != NULL)
		{
			printf("LinkedList with: ");
			linkedList *tempLL=ht->table[i];
			while(tempLL!=NULL) {
				printf("%s, ",tempLL->data->key);
				tempLL=tempLL->next;
			}
		}
        printf("\n");
	}
}

static long countAllWordsHashTable(hashTable *ht)
{
	long c=0;
	for(int i=0;i<ht->size;i++){
		c+=countAllWordsLLNode(ht->table[i]);
	}
	return c;
}

static void free_ht(hashTable *ht){
	for(int i=0;i<ht->size;i++){
		free_ll(ht->table[i]);
	}
	free(ht);
}