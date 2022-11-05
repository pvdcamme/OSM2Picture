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

extern "C" {

  typedef bool (*Callback)(size_t, const char*, const char*);
  struct TagFilter {
    const char* tag;
    const char* expected;
    Callback processor;
  };
}


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

struct MultiTagHandler: public osmium::handler::Handler {
    TagFilter* filters;
    size_t filter_cnt;
    size_t node_ctr;

    MultiTagHandler(TagFilter* _filters, size_t _filter_cnt):
      filters(_filters),
      filter_cnt(_filter_cnt),
      node_ctr(0)
    {}
        

    void node(const osmium::Node& n) {
      node_ctr++;
      for(size_t filter_ctr(0); filter_ctr < filter_cnt; ++filter_ctr) {
        auto& current = filters[filter_ctr];
        const char* tag = current.tag;
        const char* expected_val = current.expected;
        Callback callback = current.processor;

        const char* searched_val = n.get_value_by_key(tag);
        if(nullptr != searched_val && strcmp(expected_val, searched_val) == 0) {
        
          for (auto& tag: n.tags()){
            callback(node_ctr, tag.key(), tag.value());
          }
        }
      }
    }
};



void priv_filter_tags(char* file_name, struct TagFilter* filters, size_t filter_cnt) {
    osmium::io::Reader reader(file_name, osmium::osm_entity_bits::node, osmium::io::read_meta::no);
    MultiTagHandler tags(filters, filter_cnt);
    osmium::apply(reader, tags);
}

// For use with Python
extern "C" {
    int file_to_raster(char* file_name, size_t* result, size_t size)
    {
        return priv_file_to_raster(file_name, result, size);
    }

    void filter_tags(char* file_name, struct TagFilter* filters, size_t filter_cnt) {
        priv_filter_tags(file_name, filters, filter_cnt);
    }
}
