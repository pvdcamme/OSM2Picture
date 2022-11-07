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
    tag_callback_type = ctypes.CFUNCTYPE(None,ctypes.c_size_t, ctypes.c_char_p, ctypes.c_char_p)
    node_callback_type = ctypes.CFUNCTYPE(None,ctypes.c_size_t, ctypes.c_double, ctypes.c_double)

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
        _fields_ = [("tag", ctypes.c_char_p),
                    ("expected", ctypes.c_char_p),
                    ("tag_callback", tag_callback_type),
                    ("node_callback", node_callback_type)]
    
    filter_many = (TagFilter * 1)()

    place_cstr = "place".encode("ascii")
    city_cstr = "city".encode("ascii")
    filter_many[0] = TagFilter(tag=place_cstr, expected= city_cstr, tag_callback=print_city, node_callback=nodes)

    lib.filter_tags(file_name.encode("ascii"), filter_many, len(filter_many))

    result = {}
    for idx in set(cities.keys()).intersection(cities_coord.keys()):
      name = cities[idx]
      location = cities_coord[idx]
      result[name] = location

    return result 


def build_image(file_name):
    """
        Builds an image out of the PBF file
    """

    lib = ctypes.cdll.LoadLibrary(os.path.abspath("OSM2Picture.so"))

    class NodeRaster(ctypes.Structure):
        _fields_ = [("result", ctypes.POINTER(ctypes.c_size_t)),
                    ("raster_width", ctypes.c_size_t),
                    ("min_lat", ctypes.c_double),
                    ("min_lon", ctypes.c_double),
                    ("max_lat", ctypes.c_double),
                    ("max_lon", ctypes.c_double),
                    ]

    image_type = ctypes.c_size_t * (1024 * 1024)
    result = image_type()
    
    toFill = NodeRaster(result=result, raster_width=1024, min_lat=49.293, min_lon=2.208, max_lat = 51.649, max_lon =6.6)

    lib.file_to_raster2(file_name.encode("ascii"), toFill) 

    np_array = numpy.array(result).reshape((1024, 1024))
    max_val = numpy.max(np_array)
    result_image = Image.new("RGB", (1024, 1024))

    for w in range(1024):
        for h in range(1024):
            val = result[w + h * 1024]
            r, g, b = colorsys.hsv_to_rgb(val / max_val, val > 0, 1)
            result_image.putpixel((w, h),
                                  (int(255 * r), int(255 * g), int(255 * b)))
    draw = ImageDraw.Draw(result_image)
    font = ImageFont.truetype("DejaVuSans.ttf", size=20)
    for name, (lat, lon) in collect_cities(file_name).items():
      y = 1024 * (lat - toFill.min_lat) / (toFill.max_lat - toFill.min_lat)
      x = 1024 * (lon - toFill.min_lon) / (toFill.max_lon - toFill.min_lon)
      image_x, image_y= (int(x), 1024 - int(y))
      text_color = (0, 0, 0)
      draw.ellipse((image_x -4, image_y-4, image_x +3, image_y +3), fill=text_color)
      draw.text((image_x, image_y), name, fill=text_color, font = font)
      
    

    result_image.save("result.jpg")

if __name__ == "__main__":
    build_image(sys.argv[1])
