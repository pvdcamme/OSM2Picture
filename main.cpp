#include <iostream>
#include <fstream>

#include <expat.h>

#include <cstring>
#include <algorithm>

#include "OSMData.h"
#include "ImageMap.h"


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
	double minLat(180), maxLat(-180);
	double minLon(360), maxLon(-1);
	for(size_t ctr(0); ctr < ncount; ctr++)
	{
		struct OSMData::Node& n = data.getNode(ctr);
		minLat = std::min(n.lattitude, minLat);
		maxLat = std::max(n.lattitude, maxLat);
		minLon = std::min(n.longitude, minLon);
		maxLon = std::max(n.longitude, maxLon);
	}
	
	ImageMap img(1024,1024);
//	img.setArea(minLon, maxLat, maxLon, minLat);
	img.setArea(2.208, 51.649, 6.652, 49.293);
	ImageMap::iterator it = img.begin();

	while(it != img.end())
	{
		++it;
	}
	
	for(size_t ctr(0); ctr < ncount; ctr++)
	{
		struct OSMData::Node& n = data.getNode(ctr);
		img.drawPoint(n.longitude, n.lattitude,
				0, 0, 255);
	}

	img.saveImage("test.ppm");

	return true;
}
