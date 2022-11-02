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

class ImageMap;

class ImageMap
{
public:
private:
	size_t mWidth, mHeight;
	size_t* mPixels;
	
	double mXOffset, mYOffset;
	double mXScale, mYScale;

public:
	ImageMap(size_t width, size_t height);
	~ImageMap();

	ImageMap(const ImageMap&); //no copy allowed


	/* The drawing area (before transform to a bitmap)
	is defined by 2 2D point
	^
	|
	| 1   
	|
	|   
	|        2
	-------------->
	*/
	void setArea(double xTopLeft, double yTopLeft,
			double xBotRight, double yBotRight);


	void addPoint(double x, double y, size_t val);
  size_t getPoint(size_t x, size_t y);
};

#endif

