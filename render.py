import ctypes
import sys
import os

lib = ctypes.cdll.LoadLibrary(os.path.abspath("OSM2Picture.so"))
lib.file_to_raster(sys.argv[1].encode("ascii"))


