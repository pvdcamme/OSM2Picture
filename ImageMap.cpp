#include "ImageMap.h"

ImageMap::ImageMap(size_t width, size_t height):
	mWidth(width), mHeight(height),
	mPixels(new struct Color[width * height])
{
}

ImageMap::~ImageMap()
{
	delete[] mPixels;
}

void ImageMap::setArea(double xTopLeft, double yTopLeft,
		double xBotRight,double yBotRight)
{
	//dS = width / (max - min)
	//target = dS * (src - min)
}

