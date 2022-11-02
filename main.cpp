#include <iostream>

#include <algorithm>

#include "OSMData.h"
#include "ImageMap.h"


/* Start with a simple XML reader


  usage: ./<argv[0]> PBF-file

*/

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;

int priv_file_to_raster(char* file_name) 
{
    std::string name(file_name);
    OSMData data;
    data.addPbfStream(name);

    cout << "Read " << data.nodeCount() << " nodes" << endl;
    return 1;

}

extern "C" { 
int file_to_raster(char* file_name)
{
    return priv_file_to_raster(file_name);
    /*
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

    for(size_t ctr(0); ctr < ncount; ctr++)
    {
        struct OSMData::Node& n = data.getNode(ctr);
        img.drawPoint(n.longitude, n.lattitude,
                      0, 0, 255);
    }

    auto resultName = "test.ppm";
    img.saveImage(resultName);
    cout << "Wrote results to " << resultName << endl;

    */
}
}
