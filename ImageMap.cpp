#include "ImageMap.h"
#include <fstream>


using std::ofstream; //saving image to file
using std::endl;

static size_t calculateOffset(size_t mWidth,
			size_t x, size_t y)
{
	return mWidth *y + x;
}

static double rescale(double scale, double offset,
		double val)
{
	return scale * (val - offset);
}

template <typename T>
T clamp(T min, T max, T val)
{
	if(val < min)
		return min;
	if(max < val)
		return max;
	return val;
}
			
ImageMap::ImageMap(size_t width, size_t height):
	mWidth(width), mHeight(height),
	mPixels(new size_t[width * height])
{
    for(size_t ctr(0); ctr < width * height; ++ctr) {
      mPixels[ctr] = 0;
  }
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
	mXScale = mWidth / (xBotRight - xTopLeft);
	mYScale = mHeight / (yTopLeft - yBotRight);
	mXOffset = xTopLeft;
	mYOffset = yBotRight;
}


void ImageMap::drawPoint(double x, double y, size_t val)
{
	size_t scaledX = size_t(rescale(mXScale, mXOffset, x));
	size_t scaledY = size_t(rescale(mYScale, mYOffset, y));

	if(scaledX < 0 || scaledX >= mWidth)
		return;
	if(scaledY < 0 || scaledY >= mHeight)
		return;

	size_t offset = calculateOffset(mWidth, scaledX, scaledY);
	mPixels[offset] += val;
}



