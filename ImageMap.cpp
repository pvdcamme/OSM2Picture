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
	mXScale = mWidth / (xBotRight - xTopLeft);
	mYScale = mHeight / (yTopLeft - yBotRight);
	mXOffset = xTopLeft;
	mYOffset = yBotRight;
}

void ImageMap::drawPoint(double x, double y,
			unsigned char r,
			unsigned char g,
			unsigned char b)
{
	struct Color c;
	c.r = r;
	c.g = g;
	c.b = b;
	drawPoint(x,y, c);
}

void ImageMap::drawPoint(double x, double y,
		struct Color c)
{
	size_t scaledX = size_t(rescale(mXScale, mXOffset, x));
	size_t scaledY = size_t(rescale(mYScale, mYOffset, y));

	if(scaledX < 0 || scaledX >= mWidth)
		return;
	if(scaledY < 0 || scaledY >= mHeight)
		return;

	size_t offset = calculateOffset(mWidth, scaledX, scaledY);
	mPixels[offset] = c;
}

/* Saves to a PPM image
*/
bool ImageMap::saveImage(const char* name)
{
	ofstream output(name);
	output << "P3" << endl;
	output << mWidth << " ";
	output << mHeight << " ";
	output << 256 << endl;

	for(size_t y(0); y < mHeight; y++)
	{
		for(size_t x(0); x < mWidth; x++)
		{
			size_t offset = calculateOffset(mWidth, x, y);
			struct Color& c = mPixels[offset];
			output << (unsigned int)c.r << " ";
			output << (unsigned int)c.g << " ";
			output << (unsigned int)c.b << " ";
		}
		output << endl;
	}
	return output.good();
}

