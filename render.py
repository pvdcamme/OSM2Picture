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

    @ctypes.CFUNCTYPE(ctypes.c_bool,ctypes.c_size_t, ctypes.c_char_p, ctypes.c_char_p)
    def print_city(idx, tag_name, tag_val):
      print(f"{tag_name} = {tag_val.decode('utf-8')}")
    
    
    lib.visit_tags(file_name.encode("ascii"), "place".encode("ascii"), "city".encode("ascii"), print_city)
    lib.visit_tags(file_name.encode("ascii"), "place".encode("ascii"), "town".encode("ascii"), print_city)
    lib.visit_tags(file_name.encode("ascii"), "place".encode("ascii"), "village".encode("ascii"), print_city)



def build_image(file_name):
    lib = ctypes.cdll.LoadLibrary(os.path.abspath("OSM2Picture.so"))

    image_type = ctypes.c_size_t * (1024 * 1024)
    result = image_type()

    lib.file_to_raster(file_name.encode("ascii"), result, 1024)

    np_array = numpy.array(result).reshape((1024, 1024))
    max_val = numpy.max(np_array)
    result_image = Image.new("RGB", (1024, 1024))

    for w in range(1024):
        for h in range(1024):
            val = result[w + h * 1024]
            r, g, b = colorsys.hsv_to_rgb(val / max_val, val > 0, 1)
            result_image.putpixel((w, h),
                                  (int(255 * r), int(255 * g), int(255 * b)))

    result_image.save("result.jpg")


if __name__ == "__main__":
    print_cities(sys.argv[1])
    build_image(sys.argv[1])
