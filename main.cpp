#include <iostream>
#include <fstream>

#include <expat.h>

#include <cstring>

#include "OSMData.h"


/* Start with a simple XML reader


  usage: ./<argv[0]> XML-file
*/

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;

int main(int argc, char** argv)
{
	std::istream* input;
	ifstream xmlIn;

	if(argc == 1) //read from stdin;
	{
		input = &std::cin;
	}else if(argc == 2) //file argument
	{
		xmlIn.open(argv[1]);
		input = &xmlIn;
	}else
	{
		cerr << "Wrong usage" << endl;
		cerr << "expected: ";
		cerr << argv[0] << " ";
		cerr << "<xml file>" <<endl;
		return -1;
	}

	if(!xmlIn.good())
	{
		cerr << "Error: Selected bad file" << endl;
		return -2;
	}
	OSMData data;
	data.addXMLStream(*input);

	cout << "read " << data.nodeCount() << " nodes";
	cout << endl;
	size_t ncount = data.nodeCount();
	for(size_t ctr(0); ctr < ncount; ctr++)
	{
		struct OSMData::Node& n = data.getNode(ctr);
		cout << n.lattitude << " N ";
		cout << n.longitude << " E ";
		cout << endl;
	}

			
	return true;
}
