#pragma once
#ifndef SVGCONTEXT_H
#define SVGCONTEXT_H
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <nanoSVG/nanosvg.h>
#include <string.h>
#include <iostream>

using namespace std;


class SVGContext
{
public:
    SVGContext();
    ~SVGContext();

    void loadSVGFromFile(const char* path, float size);
    NSVGshape* getShapesFromImage();
    NSVGimage* getSVGImage();

    //save modified SVG Parser
    // file path may need to pass from GUI
    // Here just hard code for debug
    void saveModifiedSVGImage(const char* filePath, float width, float height);


private:
    NSVGimage* m_image;
};

#endif



