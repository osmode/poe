// 'poe.cpp' contains the main() function

#include <iostream>
#include <fstream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <set>
#include <map>
#include <cstddef>
#include <algorithm>
#include "listtools.cpp"
#include "hashtable.cpp"


//need >= MIN_WORDS_IN_SENTENCE to be considered a sentence
#define MIN_WORDS_IN_SENTENCE 3
#define MAX_WORD_LENGTH 100
#define MAX_RESULTS 50

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;
using std::multiset;
using std::map;

using HashTableOM::HashTable;

//function declarations
void cleanUp(char filename[], ifstream& stream_in, ofstream& stream_out);
void clear_array(char array_in[], int length);
string char_to_string(char array_in[], int length);
void crossOver();
int arrayKey(int a[], int size, int value );


//globals
HashTable word_list;
//words_vec is a two-dimensional vector holding each word
//in each sentence. Each sentence is represented as one vector.
vector< vector<int> > words_vec;
//words_vec2 is another two-dimensional vector resulting from
//applying the cross-over algorithm to 'words_vec'
vector< vector<int> > words_vec2;
vector<int> vectorSum(vector<int> v1, vector<int> v2);


int main(int argc, char *argv[])
{
	//list of all unique words is stored as a hash table consisting of
	//an array of pointers to class Node

	cout << endl;

	//initialize input and output filestreams
	ifstream fin;
	ofstream fout;

	//check number of arguments and end program if none 
	//are given
	switch(argc)
	{
		//case 0 should never happen
		case 0:
			cout << "argc malfunction";
			exit(1);

		//case 1: no arguments
		case 1:
			cout << "Please enter the input file name";			  cout << " as a single argument.\n"
			<< "For example: ./poe input.txt\n";
			exit(1);
		
		//case 2: one argument; use as input file name
		case 2:
			cout << "Opening file: " << argv[1];
			cout << endl;
			//string inputname = argv[1];
			cleanUp(argv[1], fin, fout); 		
						
			break;
		
		//default: more than one argument
		default:
			cout << "Too many arguments! ";	
			cout << "Please enter the input file name";			  cout << " as a single argument.\n"
			<< "For example: ./poe input.txt\n";

			exit(1);
	}
	
	crossOver();		
	
	return 0;

}	


//
void cleanUp(char filename[], ifstream& stream_in, ofstream& stream_out)
{
	//open file named 'filename', read character by character
	//and write to output file only if the character 
	//is alphanumeric
	
	//'next' stores the next character from input stream
	char next;
	//'running' stores a temporary running sentence
	string running;
	//'temp_word' stores each current word to '.put()' into the hash table
	//'temp_word_marker' stores next available array element
	char temp_word[MAX_WORD_LENGTH];
	int temp_word_marker = 0;
	clear_array(temp_word, MAX_WORD_LENGTH);
	//vector 'row' holds integers representing words
	//cleared (re-declared) anew for each sentence
	vector<int> row;

	stream_in.open(filename);
	if(stream_in.fail())
	{
		cout << "Unable to open input file stream!";
		exit(1);
	}
	stream_out.open("output");
	if(stream_out.fail())
	{
		cout << "Unable to open output file stream!";
		exit(1);
	}

	//get the first character
	stream_in.get(next);

	if(!next)
	{
		cout << "Input file is empty!" << endl;
		exit(1);
	}
	//keep '.get()'-ing until an uppercase letter is returned
	while(!isupper(next) && !stream_in.eof())
	{		
		stream_in.get(next);
	}

	//prime temp_word
	temp_word[temp_word_marker] = next;
	temp_word_marker++;

	//'uppercase_found' is used to keep track of the start
	//of a text entry
	//'end_sentence' is true when either '.' '!' or '?' + ' '
	//occur
	//'num_spaces' counts the number of spaces 
	bool uppercase_found(true), end_sentence(false), skip_once(false), get_next_sentence(false);
	unsigned int num_spaces(0);

	//keep trying to get characters from stream_in until eof
	while(!stream_in.eof())
	{
		//cout << "next: " << next;

		//get the next character from ifstream if 
		//skip_once is not flagged
		if(skip_once == false)
			stream_in.get(next);
		else
			skip_once = false;

		if(next != '\n')
		{
			running += next;
			if(isalpha(next) || (isalpha(stream_in.peek() )&&temp_word_marker>0 ) )
			{
				temp_word[temp_word_marker] = next;
				temp_word_marker++;
			}

		}

		else if(next == '\n')
		{
			stream_out << running;
			//append end-of-string before '.put()'-ing
			temp_word[temp_word_marker] = '\0'; temp_word_marker++;
			word_list.put(char_to_string(temp_word, MAX_WORD_LENGTH));
			//cout << "Adding word: " << char_to_string(temp_word, MAX_WORD_LENGTH);
			//cout << endl;

			string word_string = char_to_string(temp_word, MAX_WORD_LENGTH);
			int position = word_list.findValue(word_string);
			//push 'position' (word id) into vector 'row'
			//then push vector 'row' into words_vec
			row.push_back(position);
			
			/*
			cout << "Word \'" << word_string << "\' at position " 
				<< position << ".\n";
			*/


			temp_word_marker = 0;

			get_next_sentence = true;
			running = ' ';
			//reset temp_word_marker after '.put()'-ing a word
			num_spaces = 0;
			uppercase_found = false;
			end_sentence = false;
		}

		if(next == ' ')
		{
			num_spaces++;

			//append end-of-string before '.put()'-ing a word in 
			//the hash list
			temp_word[temp_word_marker] = '\0'; temp_word_marker++;
			word_list.put(char_to_string(temp_word, MAX_WORD_LENGTH));;
			//cout << "Adding word: " << char_to_string(temp_word, MAX_WORD_LENGTH);
			//cout << endl;

			string word_string = char_to_string(temp_word, MAX_WORD_LENGTH);
			int position = word_list.findValue(word_string);
			row.push_back(position);

			/*
			cout << "Word \'" << word_string << "\' at position " 
				<< position << ".\n";

			*/

			//cout << endl << "Counter: " << HashTable::counter;

			temp_word_marker = 0;

			//if end punctuation has already been seen
			if (end_sentence == true)
			{
				//if at least 3 words in sentence
				//a 'sentence' was found
				if(num_spaces > MIN_WORDS_IN_SENTENCE)
					stream_out << running;
				
				running = ' ';
				num_spaces = 0;
				uppercase_found = false;
				end_sentence = false;
				get_next_sentence = true;
			} //close if(end_sentence == true)

		} //close if(next == ' ')
				
		if((next == '.' || next == '!' || next == '?'))
		{
			end_sentence = true;
			temp_word[temp_word_marker] = '\0'; temp_word_marker++;
			word_list.put(char_to_string(temp_word, MAX_WORD_LENGTH));
			//cout << "Adding word: " << char_to_string(temp_word, MAX_WORD_LENGTH);
			//cout << endl;

			string word_string = char_to_string(temp_word, MAX_WORD_LENGTH);
			int position = word_list.findValue(word_string);
			row.push_back(position);

			/*
			cout << "Word \'" << word_string << "\' at position " 
				<< position << ".\n";
			*/


			temp_word_marker = 0;
		}
		//if 'get_next_sentence' was flagged, keep 
		//'.get()'-ing until a capital letter is found
		if( get_next_sentence == true )
		{
			//push_back vector 'row' into 'words_vec'
			//then clear vector 'row'
			words_vec.push_back(row);
			row.clear();

			while(!isupper(next) && !stream_in.eof())
			{
				stream_in.get(next);
			}

			skip_once = true;
			get_next_sentence=false;
		}	
			

	} //close while(!stream_in.eof())

	//close input and output stream
	stream_in.close();
	stream_out.close();

}

//Pre-condition: char array of size MAX_WORD_LENGTH
//Post-condition: same array with all elements equal to 0
void clear_array(char array_in[], int length)
{
	for( int i=0; i < length; i++)
	{
		array_in[i] = NULL;
	}

}

//Pre-condition: char array of size length that represents a word
//and ends with '\0'
//Post-condition: returns string version of input C-string
//Does not function correctly if word is not punctuated by '\0'
string char_to_string(char array_in[], int length)
{
	string out;
	bool end_of_word = false;
	int i = 0;

	for( int i = 0; i < length; i++)
	while ( (i < length) && end_of_word == false)
	{
		if(array_in[i] == '\0')
		{
			end_of_word = true;
			return out;
		}

		out += array_in[i]; 
		i++;
	}
	
	//if array_in contains an end-of-string sequence, the next line
	//should never be read;
	cout << "Missing end-of-string sequence!";
	return out;

}

//Pre-condition: a populated two-dimensional vectors filled with integers
//corresponding to words in a hash table.
//Post-condition: a new two-dimensional vector created by applying the
//cross-over algorithm to the original two-dimensional vector. A multiset
//is created from this latter 2D vector, and match scores are calculated 
//for each nested vector.   
void crossOver()
{	
	vector<int> temp;
	vector<int>::iterator temp_iter;

	vector< vector<int> >::iterator row, row2;
	vector<int>::iterator col, col2;
	//vectors 'before1' and 'after1' hold words in sentence 1
	//vectors 'before2' and 'after2' hold words in sentence 2
	//after a common word is found between two sentences
	vector<int> before1, after1, before2, after2;

for(row = words_vec.begin(); row != words_vec.end(); ++row)
{
	//at the start of each new row, clear before1
	before1.clear();

	for(col = row->begin(); col != row->end(); ++col)
	{
		
	before1.push_back(*col);

	for( row2 = row+1; row2 != words_vec.end(); ++row2)
	{
	
		//at the start of each new row, clear before2
		before2.clear();

		for(col2 = row2->begin(); col2!= row2->end(); ++col2)
		{

			before2.push_back(*col2);

			//if matching words are found, create crossover
			//and put results in words_vec2
			if(*col == *col2)
			{
				after1.clear(); after2.clear();

				//fill up after1 and after2
				for(temp_iter = col; temp_iter != row->end(); ++temp_iter)
				{
					after1.push_back(*temp_iter);
				}

				for(temp_iter = col2; temp_iter != row2->end(); ++temp_iter)
				{
					after2.push_back(*temp_iter);
				}


				words_vec2.push_back( vectorSum( before1, before2) );
				words_vec2.push_back( vectorSum( before2, after2) );

				words_vec2.push_back( vectorSum( before1, after2) );
				words_vec2.push_back( vectorSum( before2, after1) ); 

			}

		}
	}
	
	}


} //end series of nested loops and cross-over algorithm


//create multiset and populate it with every element in words_vec2
multiset<int> all_words;

for(row = words_vec2.begin(); row != words_vec2.end(); ++row)
{
	for(col = row->begin(); col != row->end(); ++col)
	{
		all_words.insert(*col);
		//cout << "Inserting " << *col << " in multiset." << endl;
	}

}

//create map with key=word id in hash table (i.e. 'counter') and
//value=match score. Output values to a temporary vector, sort,
//then print out corresonding words
map<int, int> match_scores;
temp.clear();
int mscores[HashTable::counter];

vector<int>::reverse_iterator rp;

int match_score = 0;

for(int i=0; i < HashTable::counter; i++)
{
	match_score = all_words.count(i);	
	match_scores[i] = match_score;
	mscores[i] = match_score;

	temp.push_back(match_score);

	//cout << match_score << " ";

}

//sort vector 'temp' (which contains match scores)
std::sort(temp.begin(), temp.end()-1);

//reverse-iterate through map 'match_scores'
/*
for( rp = temp.rbegin(); rp != temp.rend(); ++rp)
{
	cout << *rp << " ";

}
*/

//print out results by iterating through map match_scores
/*
typedef map<int, int>::iterator it_type;
it_type temp_iter2;

for( temp_iter2 = match_scores.begin(); temp_iter2 != match_scores.end(); ++temp_iter2 )
{
	
	cout << temp_iter2->second << " ";

}	
*/

int key = 0;

for( int i = 0; i < HashTable::counter; i++ )
{
	key = arrayKey(mscores, HashTable::counter, temp[i]);

	cout << key << " ";
	
	cout << word_list[key] << " ";


}




		
} //end crossOver


//Pre-condition: two 1D vectors passed via reference
//Post-condition: returns 1D vector that is the sum of two vectors
vector<int> vectorSum(vector<int> v1, vector<int> v2)
{
	vector<int>::iterator iter1, iter2;
	vector<int> sum;

	for( iter1 = v1.begin(); iter1 != v1.end(); ++iter1 )
		sum.push_back(*iter1);

	for( iter2 = v2.begin(); iter2 != v2.end(); ++iter2 )
		sum.push_back(*iter2);

	return sum;

}


//Pre-condition: int array 'a' with 'size' number of elements. 
//First key of array must be 0. 
//Post-condition: returns corresponding key, or -1 if no match found
int arrayKey(int a[], int size, int value )
{
	for( int i = 0; i < size; ++i )
	{
		if(a[i] == value)
		       return i;
	}

	return -1;

}

//Pre-condition: 
void removeExcludes()
{



}


