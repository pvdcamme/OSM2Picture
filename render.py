import ctypes
import sys
import os
from PIL import Image

lib = ctypes.cdll.LoadLibrary(os.path.abspath("OSM2Picture.so"))


image_type= ctypes.c_size_t * (1024 * 1024)
result = image_type()

lib.file_to_raster(sys.argv[1].encode("ascii"), result, 1024)

result_image = Image.new("RGB", (1024,1024))

for w in range(1024):
  for h in range(1024):
    val = result[w + h * 1024]
    result_image.putpixel((w,h), (0,0, 255 * (val > 0)))

result_image.save("result.jpg")    


