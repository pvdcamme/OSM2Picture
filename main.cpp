#include <iostream>
#include <fstream>

#include <expat.h>

#include <cstring>
#include <algorithm>

#include "OSMData.h"
#include "ImageMap.h"


/* Start with a simple XML reader


  usage: ./<argv[0]> XML-file
*/

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;

int main(int argc, char** argv)
{
    std::string name;

    if(argc == 2) //file argument
    {
        name = argv[1];
    } else
    {
        cerr << "Wrong usage" << endl;
        cerr << "expected: ";
        cerr << argv[0] << " ";
        cerr << "<PBF file>" <<endl;
        return -1;
    }

    OSMData data;
    data.addPbfStream(name);

    cout << "Read " << data.nodeCount() << " nodes" << endl;
    size_t ncount = data.nodeCount();
    double minLat(180), maxLat(-180);
    double minLon(360), maxLon(-1);
    for(size_t ctr(0); ctr < ncount; ctr++)
    {
        struct OSMData::Node& n = data.getNode(ctr);
        minLat = std::min(n.lattitude, minLat);
        maxLat = std::max(n.lattitude, maxLat);
        minLon = std::min(n.longitude, minLon);
        maxLon = std::max(n.longitude, maxLon);
    }

    ImageMap img(1024,1024);
    img.setArea(2.208, 51.649, 6.652, 49.293);
    ImageMap::iterator it = img.begin();

    while(it != img.end())
    {
        ++it;
    }

    for(size_t ctr(0); ctr < ncount; ctr++)
    {
        struct OSMData::Node& n = data.getNode(ctr);
        img.drawPoint(n.longitude, n.lattitude,
                      0, 0, 255);
    }

    auto resultName = "test.ppm";
    img.saveImage(resultName);
    cout << "Wrote results to " << resultName << endl;

    return true;
}
