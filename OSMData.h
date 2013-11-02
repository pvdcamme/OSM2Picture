#ifndef OSMDATA_H
#define OSMDATA_H
#include <vector>
#include <iostream>

using std::vector;
/*
  Stores OSM data in an
  easy to work with format.

  First goal to capture the node elements
  from a limited size XML stream.
*/
class OSMData
{
public:
	struct Node
	{
		unsigned long long id;
		double lattitude;
		double longitude;
	};

private:
	vector<struct Node> mNodes;

public:
	OSMData();

	bool addXMLStream(std::istream& in);

	size_t nodeCount();
	struct Node& getNode(size_t position);
};

#endif
