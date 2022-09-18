OSM2Picture
===========

Small utitlity to convert an OSM XML file to an PPM image.
Data can be read in from either an XML file or stdin.

Requirements:
* libexpat-dev for XML processing

Building and running:

$ make
$ bzcat belgium-latest.osm.bz2 |./OSM2Picture

Openstreet map files can be found at [Planet osm](https://wiki.openstreetmap.org/wiki/Planet.osm)

The result is written to test.ppm. With convert (from ImageMagick) this can be converted into 
a more common image file format.
