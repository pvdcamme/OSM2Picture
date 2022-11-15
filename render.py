"""
  This renders an Openstreetmap PBF file to JPG.
  Currently selects a bounding box that approximates belgium.

  Usage: python3 render.py <Openstreetmap PBF file>
  
"""
import ctypes
import sys
import os
from PIL import Image, ImageDraw, ImageFont
import numpy
import colorsys


def collect_cities(file_name):
    """
      Collects all cities from the file_name (pbf-file).
      Returns them as a dictionary with name -> coordinates
    """
    lib = ctypes.cdll.LoadLibrary(os.path.abspath("OSM2Picture.so"))
    tag_callback_type = ctypes.CFUNCTYPE(None, ctypes.c_size_t,
                                         ctypes.c_char_p, ctypes.c_char_p)
    node_callback_type = ctypes.CFUNCTYPE(None, ctypes.c_size_t,
                                          ctypes.c_double, ctypes.c_double)

    cities = {}
    cities_coord = {}

    @tag_callback_type
    def print_city(idx, tag_name, tag_val):
        if tag_name == b"name":
            cities[idx] = tag_val.decode('utf-8')

    @node_callback_type
    def nodes(idx, lat, lon):
        cities_coord[idx] = (lat, lon)

    class TagFilter(ctypes.Structure):
        _fields_ = [("tag", ctypes.c_char_p), ("expected", ctypes.c_char_p),
                    ("tag_callback", tag_callback_type),
                    ("node_callback", node_callback_type)]

    filter_many = (TagFilter * 1)()

    place_cstr = "place".encode("ascii")
    city_cstr = "city".encode("ascii")
    filter_many[0] = TagFilter(tag=place_cstr,
                               expected=city_cstr,
                               tag_callback=print_city,
                               node_callback=nodes)

    lib.filter_tags(file_name.encode("ascii"), filter_many, len(filter_many))

    result = {}
    for idx in set(cities.keys()).intersection(cities_coord.keys()):
        name = cities[idx]
        location = cities_coord[idx]
        result[name] = location

    return result

def build_raster(file_name, image_size, min_lat, min_lon, max_lat, max_lon):
    """
      Builds a 2D raster out of the PBF in <file_name>
      The corners of this raster are selected from the subesquent args.

      Each elements has the number of nodes whithin a the part of the raster.
    """
    lib = ctypes.cdll.LoadLibrary(os.path.abspath("OSM2Picture.so"))
    class NodeRaster(ctypes.Structure):
        _fields_ = [
            ("result", ctypes.POINTER(ctypes.c_size_t)),
            ("raster_width", ctypes.c_size_t),
            ("min_lat", ctypes.c_double),
            ("min_lon", ctypes.c_double),
            ("max_lat", ctypes.c_double),
            ("max_lon", ctypes.c_double),
        ]

    image_type = ctypes.c_size_t * (image_size * image_size)
    result = image_type()

    toFill = NodeRaster(result=result,
                        raster_width=1024,
                        min_lat= min_lat,
                        min_lon= min_lon,
                        max_lat= max_lat,
                        max_lon= max_lon)

    lib.file_to_raster2(file_name.encode("ascii"), toFill)
    return numpy.array(result).reshape((image_size, image_size))


def draw_cities(file_name, draw_image, mapper_fun):
    font = ImageFont.truetype("DejaVuSans.ttf", size=20)
    text_color = (0, 0, 0)
    for name, (lat, lon) in collect_cities(file_name).items():
        image_x, image_y = mapper_fun(lat, lon)
        draw_image.ellipse((image_x - 4, image_y - 4, image_x + 4, image_y + 4),
                     fill=text_color)
        draw_image.text((image_x, image_y), name, fill=text_color, font=font)




def build_image(file_name, output_name="result.jpg"):
    """
        Builds an image out of the PBF file and saves it to the
        output_name.
    """
    image_size = 1024
    min_lat=49.293
    min_lon=2.208
    max_lat=51.649
    max_lon=6.6

    np_array = build_raster(file_name, image_size, min_lat, min_lon, max_lat, max_lon) 
    max_val = numpy.max(np_array)
    result_image = Image.new("RGB", (image_size, image_size))

    for (x, y), val in numpy.ndenumerate(np_array):
        r, g, b = colorsys.hsv_to_rgb(val / max_val, val > 0, 1)
        result_image.putpixel((y, x),
                              (int(255 * r), int(255 * g), int(255 * b)))
    def map_to_image(lat, lon):
        y = image_size * (lat - min_lat) / (max_lat - min_lat)
        x = image_size * (lon - min_lon) / (max_lon - min_lon)
        image_x, image_y = (int(x), image_size - int(y))
        return (image_x, image_y)

    draw = ImageDraw.Draw(result_image)
    draw_cities(file_name, draw, map_to_image)
    result_image.save(output_name)


if __name__ == "__main__":
    build_image(sys.argv[1])
