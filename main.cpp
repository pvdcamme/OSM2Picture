#include <iostream>
#include <algorithm>


#include <osmium/io/pbf_input.hpp>
#include <osmium/osm.hpp>
#include <osmium/visitor.hpp>

#include <cstdint>
#include <vector>
#include "ImageMap.h"

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
    }
};

/** Collects Positions onto a raster.
 */
struct ImageHandler: public osmium::handler::Handler {
    ImageMap map;

    ImageHandler(size_t width, size_t height): map(width, height) {
        map.setArea(2.208, 51.649, 6.652, 49.293);
    }

    void node(const osmium::Node& n) {
        auto location = n.location();
        auto lat = location.lat();
        auto lon = location.lon();
        map.addPoint(lon, lat, 1);
    }
};


int priv_file_to_raster(char* file_name, size_t* result, size_t size)
{
    std::string name(file_name);

    ImageHandler mapper(size, size);
    osmium::io::Reader reader(file_name, osmium::osm_entity_bits::node, osmium::io::read_meta::no);
    osmium::apply(reader, mapper);
    for(size_t w(0); w < size; ++w) {
        for(size_t h(0); h < size; ++h) {
            result[w + h * size] = mapper.map.getPoint(w,h);
        }
    }

    return 1;
}


// For use with Python
extern "C" {
    int file_to_raster(char* file_name, size_t* result, size_t size)
    {
        return priv_file_to_raster(file_name, result, size);
    }
    void visit_tags(char* file_name, char* key, char* selected, bool (*callback)(char*)) {
    }
}
