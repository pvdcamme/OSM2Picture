#include <iostream>
#include <algorithm>


#include <osmium/io/pbf_input.hpp>
#include <osmium/osm.hpp>
#include <osmium/visitor.hpp>

#include <cstdint>
#include <vector>

using std::vector;


using std::cerr;
using std::cout;
using std::endl;



/** Counts all nodes, mostly for reporting and testing.
 */
struct CountHandler: public osmium::handler::Handler {
  size_t counted;

  CountHandler(): counted(0) {}

  void node(const osmium::Node& n) {
    counted++;
    /*
    auto location = n.location();
    auto lat = location.lat();
    auto lon = location.lon();
    struct OSMData::Node saved;
    saved.id = n.id();
    saved.lattitude = lat;
    saved.longitude = lon;
    nodes.push_back(saved);
    */
  }
};


/* Start with a simple XML reader


  usage: ./<argv[0]> PBF-file

*/
int priv_file_to_raster(char* file_name) 
{
    std::string name(file_name);

    CountHandler collector;
    osmium::io::Reader reader(file_name, osmium::osm_entity_bits::node, osmium::io::read_meta::no);
    osmium::apply(reader, collector);

    cout << "Read " << collector.counted << " nodes" << endl;
    return 1;

}

extern "C" { 
int file_to_raster(char* file_name)
{
    return priv_file_to_raster(file_name);
    /*
    for(size_t ctr(0); ctr < ncount; ctr++)
    {
        struct OSMData::Node& n = data.getNode(ctr);
        minLat = std::min(n.lattitude, minLat);
        maxLat = std::max(n.lattitude, maxLat);
        minLon = std::min(n.longitude, minLon);
        maxLon = std::max(n.longitude, maxLon);
    }

    ImageMap img(1024,1024);
    img.setArea(2.208, 51.649, 6.652, 49.293);
    ImageMap::iterator it = img.begin();

    for(size_t ctr(0); ctr < ncount; ctr++)
    {
        struct OSMData::Node& n = data.getNode(ctr);
        img.drawPoint(n.longitude, n.lattitude,
                      0, 0, 255);
    }

    auto resultName = "test.ppm";
    img.saveImage(resultName);
    cout << "Wrote results to " << resultName << endl;

    */
}
}
