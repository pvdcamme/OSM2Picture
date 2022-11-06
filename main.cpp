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

  typedef void (*TagCallback)(size_t, const char*, const char*);
  typedef void (*NodeCallback)(size_t, double, double);
  struct TagFilter {
    const char* tag;
    const char* expected;
    TagCallback processor;
    NodeCallback nodes;
  };

  struct NodeRaster {
    size_t* result;
    size_t raster_width;
    double min_lat;
    double min_lon;
    double max_lat;
    double max_lon;
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

    void setArea(double min_longitude, double min_latitude, double max_longitude, double max_latitude){
      map.setArea(min_longitude, min_latitude, max_longitude, max_latitude);
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

int priv_file_to_raster2(char* file_name, struct NodeRaster raster) {
    std::string name(file_name);

    size_t size =raster.raster_width;

    ImageHandler mapper(size, size);
    mapper.setArea(raster.min_lon, raster.min_lat, raster.max_lon, raster.max_lat);
    osmium::io::Reader reader(file_name, osmium::osm_entity_bits::node, osmium::io::read_meta::no);
    osmium::apply(reader, mapper);

    for(size_t w(0); w < size; ++w) {
        for(size_t h(0); h < size; ++h) {
            raster.result[w + h * size] = mapper.map.getPoint(w,h);
        }
    }

  return 1;
}

struct MultiTagHandler: public osmium::handler::Handler {
    TagFilter* filters;
    size_t filter_cnt;

    MultiTagHandler(TagFilter* _filters, size_t _filter_cnt):
      filters(_filters),
      filter_cnt(_filter_cnt)
    {}


    void node(const osmium::Node& n) {
      for(size_t filter_ctr(0); filter_ctr < filter_cnt; ++filter_ctr) {
        auto& current = filters[filter_ctr];

        if( matches_node(n, current)) {
          notify_node(n, current);
          notify_filter(n, current);
        }
      }
    }

private:
    bool matches_node(const osmium::Node& n, TagFilter& filter) const{
        const char* tag = filter.tag;
        const char* expected_val = filter.expected;

        const char* searched_val = n.get_value_by_key(tag);
        return nullptr != searched_val && strcmp(expected_val, searched_val) == 0;
    }


    void notify_node(const osmium::Node& n, TagFilter& filter) {
          auto location = n.location();
          auto lat = location.lat();
          auto lon = location.lon();
          filter.nodes(n.id(), lat, lon);
    }
    void notify_filter(const osmium::Node& n, TagFilter& filter) {
          TagCallback callback = filter.processor;
          size_t id = n.id();
          for (auto& tag: n.tags()){
            callback(id, tag.key(), tag.value());
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

    int file_to_raster2(char* file_name, struct NodeRaster raster) {
      return priv_file_to_raster2(file_name, raster);
    }

    void filter_tags(char* file_name, struct TagFilter* filters, size_t filter_cnt) {
        priv_filter_tags(file_name, filters, filter_cnt);
    }
}
