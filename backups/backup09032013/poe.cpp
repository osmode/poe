// 'poe.cpp' contains the main() function

#include <iostream>
#include <fstream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <cstddef>
#include "listtools.cpp"
#include "hashtable.cpp"


//need >= MIN_WORDS_IN_SENTENCE to be considered a sentence
#define MIN_WORDS_IN_SENTENCE 3
#define MAX_WORD_LENGTH 100

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::string;

using HashTableOM::HashTable;

void cleanUp(char filename[], ifstream& stream_in, ofstream& stream_out);
void clear_array(char array_in[], int length);
string char_to_string(char array_in[], int length);


HashTable word_list;

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
	bool uppercase_found(true), end_sentence(false), skip_once(true), get_next_sentence(false);
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
			if(isalpha(next))
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
			cout << "Adding word: " << char_to_string(temp_word, MAX_WORD_LENGTH);
			cout << endl;

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
			cout << "Adding word: " << char_to_string(temp_word, MAX_WORD_LENGTH);
			cout << endl << "Counter: " << HashTable::counter;
			cout << endl;

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
			cout << "Adding word: " << char_to_string(temp_word, MAX_WORD_LENGTH);
			cout << endl;

			temp_word_marker = 0;
		}
		//if 'get_next_sentence' was flagged, keep 
		//'.get()'-ing until a capital letter is found
		if( get_next_sentence == true )
		{
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

		


