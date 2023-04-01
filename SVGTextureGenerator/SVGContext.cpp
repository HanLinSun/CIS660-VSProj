#define NANOSVG_IMPLEMENTATION
#include "SVGContext.h"
#include <string.h>
#include <fstream>

SVGContext::SVGContext() {}
SVGContext::~SVGContext() {}
NSVGshape* SVGContext::getShapesFromImage()
{
    return m_image->shapes;
}

NSVGimage* SVGContext::getSVGImage()
{
    return m_image;
}

void SVGContext::loadSVGFromFile(const char* path, float size)
{
    m_image = nsvgParseFromFile(path, "px", size);
    if (m_image)
    {
        printf("Load Success \n");
    }
    else
    {
        printf("Load failed \n");
    }
}

void SVGContext::saveModifiedSVGImage(const char* path, float width, float height)
{

    std::ofstream file(path);
    std::string headerStr = "<?xml version=\"1.0\" standalone=\" no\"?> \n";


    string widthStr = "\"" + std::to_string(width) + "\"";
    string heightStr = "\"" + std::to_string(height) + "\"";
    string configStr = "<svg width=" + widthStr + "height=" + heightStr + " version=\"1.1\"> \n";


    string str_1 = "<rect x=\"10\" y=\"10\" width=\"30\" height=\"30\" stroke=\"black\" fill=\"transparent\" stroke-width=\"5\"/> \n";
    str_1 += "<rect x=\"60\" y=\"10\" rx=\"10\" ry=\"10\" width=\"30\" height=\"30\" stroke=\"black\" fill=\"transparent\" stroke-width=\"5\"/>\n";
    str_1 += "<rect x=\"80\" y=\"40\" rx=\"10\" ry=\"10\" width=\"20\" height=\"20\" stroke=\"black\" fill=\"transparent\" stroke-width=\"5\"/>\n";
    str_1 += "<circle cx=\"25\" cy=\"10\" r=\"20\" stroke=\"red\" fill=\"transparent\" stroke-width=\"5\"/> \n";
    str_1 += "<circle cx=\"65\" cy=\"10\" r=\"20\" stroke=\"red\" fill=\"transparent\" stroke-width=\"5\"/> \n";
    str_1 += "<ellipse cx=\"75\" cy=\"10\" rx=\"20\" ry=\"5\" stroke=\"red\" fill=\"transparent\" stroke-width=\"5\"/> \n";

    str_1 += "<line x1=\"100\" x2=\"150\" y1=\"40\" y2=\"90\" stroke=\"orange\" stroke-width=\"5\"/> \n";
    str_1 += "<polyline points=\"60 110 65 120 70 115 75 130 80 125 85 140 90 135 95 150 100 145\" stroke=\"orange\" fill=\"transparent\" stroke-width=\"5\"/> \n";

    str_1 += "<path d=\"M20,230 Q40,205 50,230 T90,230\" fill=\"none\" stroke=\"blue\" stroke-width=\"5\"/> \n";

    headerStr += configStr + str_1 + "</svg> \n";

    file << headerStr;

    file.close();
}

