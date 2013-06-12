class Input
{
public:
	Input();
	//filetext is the name of the input text file
	//excludes is the name of the exclusion words file
	Input(string filetext, string excludes);
	void cleanText();

private:
	//sentences are stored as string vectors
	vector<string> sentences;
	//words are encoded as integers 
	// *words is a dynamic array of integers
	int *words;
	int *words_lowercase;

};


