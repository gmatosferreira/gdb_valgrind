#
# makefile to compile all programs for the AED.P08 computer lab class
#

clean:
	rm -f a.out
	rm -f count_words
	rm -f badWords
	rm -f binaryTree
	rm -f hashTable

.PHONY:		all
all:		hashTable count_words

count_words:	count_words.c hashTable.h binaryTree.h badWords.h
	cc -Wall -g count_words.c -o count_words

hashTable: hashTable.c
	cc -Wall -g hashTable.c -o hashTable
