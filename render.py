"""
  This renders an Openstreetmap PBF file to JPG.
  Currently selects a bounding box that approximates belgium.

  Usage: python3 render.py <Openstreetmap PBF file>
  
"""
import ctypes
import sys
import os
from PIL import Image
import numpy
import colorsys


def print_cities(file_name):
    lib = ctypes.cdll.LoadLibrary(os.path.abspath("OSM2Picture.so"))

    callback_type = ctypes.CFUNCTYPE(ctypes.c_bool,ctypes.c_size_t, ctypes.c_char_p, ctypes.c_char_p)

    @ctypes.CFUNCTYPE(ctypes.c_bool,ctypes.c_size_t, ctypes.c_char_p, ctypes.c_char_p)
    def print_city(idx, tag_name, tag_val):
      print(f"{idx} :: {tag_name} = {tag_val.decode('utf-8')}")
      return True

    class TagFilter(ctypes.Structure):
        _fields_ = [("tag", ctypes.c_char_p),
                    ("expected", ctypes.c_char_p),
                    ("callback", callback_type)]
    
    filter_many = (TagFilter * 3)()

    ## Explicit, to ensure vals are not GC-ed
    place_cstr = "place".encode("ascii")
    city_cstr = "city".encode("ascii")
    town_cstr = "tonw".encode("ascii")
    village_cstr = "village".encode("ascii")
    filter_many[0] = TagFilter(tag=place_cstr, expected= city_cstr, callback=print_city)
    filter_many[1] = TagFilter(tag=place_cstr, expected= town_cstr, callback=print_city)
    filter_many[2] = TagFilter(tag=place_cstr, expected= village_cstr, callback=print_city)

    lib.filter_tags(file_name.encode("ascii"), filter_many, len(filter_many))







def build_image(file_name):
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
    print(f"Maxval : {max_val}")

    for w in range(1024):
        for h in range(1024):
            val = result[w + h * 1024]
            r, g, b = colorsys.hsv_to_rgb(val / max_val, val > 0, 1)
            result_image.putpixel((w, h),
                                  (int(255 * r), int(255 * g), int(255 * b)))

    result_image.save("result.jpg")


if __name__ == "__main__":
    #print_cities(sys.argv[1])
    build_image(sys.argv[1])
