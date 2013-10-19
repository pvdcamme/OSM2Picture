#include <iostream>
#include <fstream>

#include <expat.h>


/* Start with a simple XML reader


  usage: ./<argv[0]> XML-file
*/

using std::cerr;
using std::endl;

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

	std::ifstream xmlIn(argv[1]);
	if(!xmlIn.good())
	{
		cerr << "Select bad file" << endl;
		return -2;
	}

	XML_Parser xmlEvt =  XML_ParserCreate(NULL);
	XML_ParserFree(xmlEvt);

		
	return true;
}
