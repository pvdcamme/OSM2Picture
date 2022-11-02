#include <osmium/io/pbf_input.hpp>
#include <osmium/osm.hpp>
#include <osmium/visitor.hpp>

#include <cstdint>

#include "OSMData.h"


/** Collects all nodes for later usage.
 */
struct CountHandler: public osmium::handler::Handler {
  size_t counted;
  
  CountHandler(): counted(0) 
  {}

  void node(const osmium::Node& n) {
    ++counted;
  }
};


/** Collects all nodes for later usage.
 */
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
  mFileNames.push_back(file_name);
  osmium::io::Reader reader(file_name, osmium::osm_entity_bits::node, osmium::io::read_meta::no);

  CollectHandler handler;
  osmium::apply(reader, handler);
  mNodes = handler.nodes;
	return true;
}

size_t OSMData::nodeCount()
{
  CountHandler totalCount;
  for(auto fileName: mFileNames) {
    osmium::io::Reader reader(fileName, osmium::osm_entity_bits::node, osmium::io::read_meta::no);
    osmium::apply(reader, totalCount);
  }
	return totalCount.counted;
}

struct OSMData::Node& OSMData::getNode(size_t position)
{
	return mNodes[position];
}
