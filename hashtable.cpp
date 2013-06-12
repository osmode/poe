//implementation file 'hashtable.cpp'
//implementatino of class HashTable

#include <string>
#include "listtools.h"
#include "hashtable.h"

using LinkedListOM::Node;
using LinkedListOM::search;
using LinkedListOM::headInsert;
using std::string;

namespace HashTableOM
{
	HashTable::HashTable()
	{
		for( int i=0; i < SIZE; i++)
		{
			hashArray[i] = NULL;
		}
	}

	HashTable::~HashTable()
	{
		for( int i=0; i < SIZE; i++)
		{
			Node<string> *next = hashArray[i];
			while (next != NULL)
			{
				Node<string> *discard = next;
				next = next->getLink();
				//delete object 'next's pointer '*link' to prevent dangling pointers
				delete discard;
			}
		}
	}

	//function that calculates hash values to correspond 
	//to array keys
	int HashTable::computeHash(string s)
	{
		int hash = 0;
		for( int i=0; i < s.length(); i++)
			hash += s[i];
		
		return hash;
	}

	// 'put' inserts a word  in the hash table as a string
	// step 1: search hash table to existence of string 's'
	// step 2: if not already in hash table, add it and 
	// increment HashTable::counter 
	void HashTable::put(string s)
	{
		bool does_not_already_exist = true;

		for(int i = 0; i < SIZE; i++)
		{
			if( search(hashArray[i], s) != NULL)
				does_not_already_exist = false;
		}
			
		if( does_not_already_exist == true )
		{
			headInsert(hashArray[counter], s);
			counter++;
		}

		
	}

	int HashTable::findValue(string s) const
	{
		//uses static int 'counter' in class definition
		for( int i=0; i < counter; i++)
		{
			//if value 's' exists
			if(search( hashArray[i], s) != NULL)
			{
				//return integer representing
				//unique word id number
				return i;
			}
		}

		//if value 's' is not in the hashArray after 
		//iterating through it, return -1
		return -1;
	}


	string HashTable::operator[](int index)
	{
		return hashArray[index]->getData();
	}



} //close namespace 'HashTableOM'


