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
public:
	struct Color{
		unsigned char r;
		unsigned char g;
		unsigned char b;
	};
private:
	size_t mWidth, mHeight;
	struct Color* mPixels;
	
	double mXOffset, mYOffset;
	double mXScale, mYScale;

public:
	ImageMap(size_t width, size_t height);
	~ImageMap();

	ImageMap(const ImageMap&); //no copy allowed


	/* The drawing area (before transform to a bitmap)
	is defined by 2 2D point
	*/
	void setArea(double xTopLeft, double yTopLeft,
			double xBotRight, double yBotRight);


	void drawPoint(double x, double y,
			unsigned char r,
			unsigned char g,
			unsigned char b);
	void drawPoint(double x, double y,
		struct Color c);
	
	bool saveImage(char* name);
};
#endif

