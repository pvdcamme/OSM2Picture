import ctypes
import sys
import os

lib = ctypes.cdll.LoadLibrary(os.path.abspath("OSM2Picture.so"))


image_type= ctypes.c_size_t * 1024 * 1024
result = image_type()

lib.file_to_raster(sys.argv[1].encode("ascii"), result, 1024)


