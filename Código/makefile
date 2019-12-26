#
# makefile to compile all programs for the AED.P08 computer lab class
#

clean:
	rm -f a.out
	rm -f count_words

.PHONY:		all
all:		hashTable count_words

count_words:	count_words.c
	cc -Wall -g count_words.c -o count_words

hashTable: hashTable.c
	cc -Wall -g hashTable.c -o hashTable
