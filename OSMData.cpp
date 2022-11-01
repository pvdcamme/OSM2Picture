#include <iostream>
#include <osmium/io/pbf_input.hpp>
#include <osmium/handler.hpp>
#include <osmium/visitor.hpp>
#include <cstdint>

#include "OSMData.h"

using std::ifstream;


OSMData::OSMData()
{
}

struct CollectHandler: public osmium::handler::Handler {
  vector<OSMData::Node> nodes;

  void node(const osmium::Node& n) {
    auto location = n.location();
    auto lat = location.lat();
    auto lon = location.lon();
    struct OSMData::Node saved;
    saved.id = n.id();
    saved.lattitude = lat;
    saved.longitude = lon;
    nodes.push_back(saved);
  }
  
};


bool OSMData::addPbfStream(std::string& file_name)
{
  osmium::io::Reader reader(file_name);

  CollectHandler handler;
  osmium::apply(reader, handler);
  mNodes = handler.nodes;
	return true;
}

size_t OSMData::nodeCount()
{
	return mNodes.size();
}

struct OSMData::Node& OSMData::getNode(size_t position)
{
	return mNodes[position];
}
