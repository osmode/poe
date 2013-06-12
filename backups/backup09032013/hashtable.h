//header file 'hashtable.h'
//interface for the class HashTable, which is a class for a
//hash table of strings
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include "listtools.h"

using LinkedListOM::Node;
using std::string;

namespace HashTableOM
{

const int SIZE = 1000;

class HashTable{

public:
	//initializes empty hash table 
	HashTable();

	//destructor
	virtual ~HashTable();

	//returns true if target is in hash table
	//false otherwise
	bool containsString(string target) const;

	void put(string s);
	//counter is the value given to each word in the hash
	//table and is incremented with each new entry
	static int counter;

private:
	//hash table exists as an array with each key
	//corresponding to a computed hash value
	//and each value a unique integer 
	Node<string> *hashArray[SIZE];
	static int computeHash(string s);

};

int HashTable::counter = 0;

} // close namespace HashTableOM

#endif


