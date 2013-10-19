#include <iostream>
#include <fstream>

#include <expat.h>

#include <cstring>


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


void OSMStartElement(void * userdata,
		const XML_Char *name,
		const XML_Char **atts)
{
	if (strcmp(name, "node") != 0)
		return;

	unsigned long int ID;
	double lattitude;
	double longitude;
	const XML_Char** END_REF = {NULL};
	while(*atts != NULL)	
	{
		const XML_Char* key = *atts;
		const XML_Char* value = *(atts + 1);
		atts += 2;
		if(key && value)
			cout << key << " :: " << value << " -- ";
		else
			atts = END_REF; //incomplete key-val pair
	}
	cout << endl;
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
	XML_SetStartElementHandler(parser, OSMStartElement);
	XML_Status parseResult = parseFile(parser, xmlIn);
	cout << "Did parse correctly? ";
	cout << std::boolalpha << (parseResult == XML_STATUS_OK);
	cout << endl;

	XML_ParserFree(parser);

		
	return true;
}
