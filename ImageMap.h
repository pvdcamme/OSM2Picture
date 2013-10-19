#ifndef IMAGE_MAP
#define IMAGE_MAP

#include <cstddef> //for size_t
/*
 The ImageMap class maps a predefined linear scaling
 to an image (field).

 A general use-case is following:
 	1) Create the ImageMap to represent a fixed-size image.
	2) set the scaling/offset factor. This in essence.
	rescales/repositions the area where is drawn on.
	3) draw a lot of points.
	4) save the resulting image to a file.
*/
class ImageMap
{
	size_t mWidth, mHeight;
public:
	ImageMap(size_t width, size_t height);
	~ImageMap();

	void setScale(double x, double y);
	void setOffset(double x, double y);


	void drawPoint(double x, double y,
			unsigned char r,
			unsigned char g,
			unsigned char b);
	
	bool saveImage(char* name);
};
#endif

