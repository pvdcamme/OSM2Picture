#include <iostream>
#include <fstream>

#include <expat.h>


/* Start with a simple XML reader


  usage: ./<argv[0]> XML-file
*/

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;


XML_Status parseFile(XML_Parser& parser, 
		ifstream& input)
{
	const size_t BUFFERSIZE = 2048;
	char buffer[BUFFERSIZE];
	size_t readAmt = 1; //init value for loop

	while(input.good() && readAmt > 0)
	{
		input.read(buffer, BUFFERSIZE);
		readAmt = input.gcount();
		XML_Parse(parser, buffer, readAmt, false);
	}
	return XML_Parse(parser, buffer, 0, true);
}

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		cerr << "Wrong usage" << endl;
		cerr << "expected: ";
		cerr << argv[0] << " ";
		cerr << "<xml file>" <<endl;
		return -1;
	}

	ifstream xmlIn(argv[1]);
	if(!xmlIn.good())
	{
		cerr << "Select bad file" << endl;
		return -2;
	}

	XML_Parser parser =  XML_ParserCreate(NULL);
	XML_Status parseResult = parseFile(parser, xmlIn);
	cout << "Did parse correctly? ";
	cout << std::boolalpha << (parseResult == XML_STATUS_OK);
	cout << endl;

	XML_ParserFree(parser);

		
	return true;
}
