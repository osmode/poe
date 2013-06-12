//default constructor will require names of input txt file 
//and excludes file	
Input::Input()
{
	cout << endl;
	cout << "Constructor requires names of input file and " 
		<< "excludes file as parameters.\n";
	exit(1);
}

//class Input constructor takes 2 parameters 'filetext'
//and 'excludes' as strings passed by value and attempts
//to open the corresponding file name to extract data 
Input::Input(string filetext, string excludes)
{
	//two file streams, one for input text, one for excludes
	ifstream inText, excludesText;
	inText.open(filetext);
	if(inText.fail())
	{
		cout << endl;
		cout << "Unable to open input file named \'" 
		<< filetext << "\'\n";	
		cout << endl;

		exit(1);
	}
	excludesText.open(excludes);
	if(excludesText.fail())
	{
		cout << endl;
		cout << "Unable to open excludes file named \'"
		<< excludes << "\'\n";
		cout << endl;

		exit(1);
	}

	
