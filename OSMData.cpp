#include <iostream>
#include <fstream>

#include <cstring>
#include <expat.h>

#include "OSMData.h"

using std::ifstream;

OSMData::OSMData()
{
}

/*
	prints out the nodes
*/
static void OSMStartElement(void * userdata,
		const XML_Char *name,
		const XML_Char **atts)
{
	if (strcmp(name, "node") != 0)
		return;

	long long int ID = -1;
	double lattitude = -1;
	double longitude = -1;
	while(*atts != NULL)	
	{
		const XML_Char* key = *atts;
		atts++;
		const XML_Char* value = *atts;
		if(value == NULL)
			break;
		atts++;
		if(strcmp(key, "id") == 0)
			ID = strtoll(value, NULL, 10);
		else if(strcmp(key, "lat") == 0)
			lattitude = strtod(value, NULL);
		else if(strcmp(key, "lon") == 0)
			longitude = strtod(value, NULL);
	}
	if(ID != -1) 
	{
		vector<struct OSMData::Node>* nodes = 
			(vector<struct OSMData::Node>*) userdata;
		struct OSMData::Node n;
		n.id = ID;
		n.lattitude = lattitude;
		n.longitude = longitude;
		nodes->push_back(n);
	}
}
/* Reads in a full stream passes its
   data to the XML parser.
 */
bool OSMData::addXMLStream(std::istream& input)
{
	const size_t BUFFERSIZE = 32*1024;
	char buffer[BUFFERSIZE];
	size_t readAmt = 1; //init value for loop
	bool result(false);

	XML_Parser parser =  XML_ParserCreate(NULL);
	XML_SetUserData(parser, &mNodes);
	XML_SetStartElementHandler(parser, OSMStartElement);

	while(input.good() && readAmt > 0)
	{
		input.read(buffer, BUFFERSIZE);
		readAmt = input.gcount();
		XML_Parse(parser, buffer, readAmt, false);
	}

	result = (XML_STATUS_OK == XML_Parse(parser, buffer, 0, true));
	XML_ParserFree(parser);
	return result;
}

size_t OSMData::nodeCount()
{
	return mNodes.size();
}

struct OSMData::Node& OSMData::getNode(size_t position)
{
	return mNodes[position];
}
