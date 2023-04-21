#pragma once
#ifndef SAMPLE_METHOD_HH
#define SAMPLE_METHOD_HH

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <nanoSVG/nanosvg.h>
#include <vector>
//====== Add by Hanlin =====

struct point
{
    glm::vec2 position;
    glm::vec3 color;
};

using namespace std;

class SampleMethod
{
public:
    SampleMethod();
    ~SampleMethod();

    static SampleMethod* getInstance()
    {
        if (instance == NULL)
        {
           instance = new SampleMethod();
        }
        return instance;
    }

    SampleMethod& operator=(const  SampleMethod&);

    bool isValidPoint(vector<vector<glm::vec2>>& grids, float cellSize, int canvas_width, int canvas_height, glm::vec2 point, float radius);

    /// <summary>
    /// This method is used to sample the whole canvas
    /// </summary>
    /// <param name="radius"> minimum distance between points</param>
    /// <param name="k">uplimit of point number</param>
    /// <param name = "canvas xxx"> SVG canvas's size(width and height) </param>
    /// <returns></returns>
    /// 
   vector<point> poissionDiskSampling(float radius, float k, int canvas_width, int canvas_height,glm::vec2 canvas_pos,glm::vec3 fillCol, glm::vec3 strokeCol,NSVGpath* path, float strokeWidth);

   void insertSamplePoint(glm::vec2 point,  vector<vector<glm::vec2>>& grids,  float cellSize);
   void turnBezierIntoPoly(NSVGpath* path);

   bool pointInsidePoly(glm::vec2 point,float strokeWidth);

   vector<glm::vec2> polygonPoints;
   vector<glm::vec2> hierarchicalSampling();

private:
  inline static SampleMethod* instance=NULL;
};

#endif