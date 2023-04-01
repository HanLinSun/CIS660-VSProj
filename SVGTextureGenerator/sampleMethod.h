#pragma once
#ifndef SAMPLE_METHOD_HH
#define SAMPLE_METHOD_HH

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <vector>
//====== Add by Hanlin =====
using namespace std;

class SampleMethod
{
public:
    SampleMethod();
    ~SampleMethod();

    static SampleMethod& getInstance()
    {
        return instance;
    }

    SampleMethod& operator=(const  SampleMethod&);

    bool isValidPoint(vector<glm::vec2> grids, float cellSize, int canvas_width, int canvas_height, glm::vec2 point, float radius);

    /// <summary>
    /// This method is used to sample the whole canvas
    /// </summary>
    /// <param name="radius"> minimum distance between points</param>
    /// <param name="k">uplimit of point number</param>
    /// <param name = "canvas xxx"> SVG canvas's size(width and height) </param>
    /// <returns></returns>
    /// 
   vector<glm::vec2> poissionDiskSampling(float radius, float k, int canvas_width, int canvas_height);

   void insertSamplePoint(glm::vec2 point,  vector<vector<glm::vec2>>& grids,  float cellSize);

   vector<glm::vec2> hierarchicalSampling();

private:
    static SampleMethod instance;
};

#endif