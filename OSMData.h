#ifndef OSMDATA_H
#define OSMDATA_H
#include <vector>
#include <iostream>
#include <string>


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
  class Iter{
    private:
      bool atEnd;
      Iter(OSMData&);

    public:
      struct Node& operator*();
      Iter& operator++();
  };

private:
	vector<struct Node> mNodes;
  vector<std::string> mFileNames;

public:

	bool addPbfStream(std::string& fileName);

	size_t nodeCount();

	struct Node& getNode(size_t position);


};

#endif
