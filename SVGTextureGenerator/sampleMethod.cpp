#include "sampleMethod.h"
#include <stdlib.h> 
#include <random>
#include <nanoSVG/nanosvg.h>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <math.h>

#define SAMPLE_COUNT 15
#define EPSILON 0.000001

SampleMethod::SampleMethod() {}
SampleMethod::~SampleMethod() {}

void SampleMethod::insertSamplePoint(glm::vec2 point, vector<vector<glm::vec2>>& grids,float cellSize)
{
	int xIndex = floor(point.x / cellSize);
	int yIndex = floor(point.y / cellSize);
	//put this point into corresponding grid
	grids[yIndex][xIndex] = point;
}


float computeCubicBezierCurve(float p0, float p1, float p2, float p3,float& t)
{
	float coeff_1 = (float)pow(1 - t, 3);
	float coeff_2 = 3 * t * (float)pow(1 - t, 2);
	float coeff_3 = 3 * (1 - t) * t * t;
	float coeff_4 = (float)pow(t, 3);

	return p0 * coeff_1 + p1 * coeff_2 + p2 * coeff_3 + p3 * coeff_4;
}

glm::vec2 getBezierCurvePoint(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, float t)
{
	float x = computeCubicBezierCurve(p0.x, p1.x, p2.x,p3.x, t);
	float y = computeCubicBezierCurve(p0.y, p1.y, p2.y, p3.y, t);
	return glm::vec2(x, y);
}

void SampleMethod::turnBezierIntoPoly(NSVGpath* path)
{
	int npts = path->npts;
	float* pts = path->pts;

	//why square have 26 value, divide by two have 13 (0 25)


	glm::vec2 startPoint;
	glm::vec2 endPoint;

	for (int i = 0; i < npts-1; i+=3)
	{
		float* p = &pts[i * 2];
		//p0
		float p0 = p[0];
		float p1 = p[1];
		
		glm::vec2 p_0 = glm::vec2(p0, p1);
		//p1
		float p2 = p[2];
		float p3 = p[3];
		glm::vec2 p_1 = glm::vec2(p2, p3);
		//p2
		float p4 = p[4];
		float p5 = p[5];
		glm::vec2 p_2 = glm::vec2(p4, p5);
		//p3
		float p6 = p[6];
		float p7 = p[7];
		glm::vec2 p_3 = glm::vec2(p6, p7);

		//for each bezier curve sample 15 times
		for (float i = 0.0f; i < SAMPLE_COUNT; i+=1.0f)
		{
			glm::vec2 polygonPoint = getBezierCurvePoint(p_0, p_1, p_2, p_3, i / SAMPLE_COUNT);
			polygonPoints.push_back(polygonPoint);
		}
	}
	//now have the polygon
}
bool isPointIntersectable(glm::vec2 edge_1, glm::vec2 edge_2, glm::vec2 point)
{
	float max_y = std::max(edge_1.y, edge_2.y);
	float min_y = std::min(edge_1.y, edge_2.y);
	if (point.y > max_y || point.y < min_y)
	{
		return false;
	}
	else return true;
}

bool isPointInRange(glm::vec2 edge_1, glm::vec2 edge_2, glm::vec2 point)
{
	float max_y = std::max(edge_1.y, edge_2.y);
	float min_y = std::min(edge_1.y, edge_2.y);

	float max_x = std::max(edge_1.x, edge_2.x);
	float min_x = std::min(edge_1.x, edge_2.x);

	if (point.y > max_y || point.y < min_y || point.x>max_x ||point.x<min_x)
	{
		return false;
	}
	else return true;
}


bool IsPointOnLine(glm::vec2 edge_1, glm::vec2 edge_2, glm::vec2 point)
{
	glm::vec2 vec_1 = point - edge_1;
	glm::vec2 vec_2 = point - edge_2;

	//cross product(x1y2-x2y1)
	if ((vec_1.x * vec_2.y - vec_2.x * vec_1.y) == 0)
	{
		//same line
		if (isPointInRange(edge_1, edge_2, point))
		{
			return true;
		}
	}
	else return false;
}

float crossProduct(glm::vec2 v1, glm::vec2 v2)
{
	return v1.x * v2.y - v2.x * v1.y;
}
//Line is horizontal
bool IsIntersect(glm::vec2 edge_1, glm::vec2 edge_2,glm::vec2 rayOrigin, glm::vec2 rayEnd)
{
	//use cross product to judge
	// the two line should both cross each other
	glm::vec2 v1 = edge_1 - rayEnd;
	glm::vec2 v2 = edge_2 - rayEnd;
	glm::vec2 v3 = rayEnd - rayOrigin;

	glm::vec2 v4 = rayOrigin - edge_1;
	glm::vec2 v5 = rayEnd - edge_1;
	glm::vec2 v6 = edge_2 - edge_1;

	

	float num1 = crossProduct(v1, v3);
	float num2 = crossProduct(v2, v3);

	float num3 = crossProduct(v4, v6);
	float num4 = crossProduct(v5, v6);

	if ((num1 * num2 <= 0 && num3 * num4 < 0) || (num3 * num4 <= 0 && num1 * num2 < 0))
	{
		return true;
	}
	else return false;
}

bool SampleMethod::pointInsidePoly(glm::vec2 point,float strokeWidth)
{
	//ray intersect

	bool isInside = false;
	int count = 0;
	float minX = FLT_MAX;

	for (int i = 0; i < polygonPoints.size(); i++)
	{
		minX = std::min(minX, polygonPoints[i].x);
	}

	float px = point.x;
	float py = point.y;

	//horizontal ray
	glm::vec2 rayOrigin = glm::vec2(point.x, point.y);
	glm::vec2 rayEnd = glm::vec2(minX - 10, point.y);

	//launch a ray on x level, end point is the minimum x -10 to make sure intersect


	//iterate through every edge
	for (int i = 0; i < polygonPoints.size() - 1; i++)
	{
		glm::vec2 polyEdge_1 = glm::vec2(polygonPoints[i].x, polygonPoints[i].y);
		glm::vec2 polyEdge_2 = glm::vec2(polygonPoints[i+1].x, polygonPoints[i+1].y);

		if (isPointIntersectable(polyEdge_1, polyEdge_2, rayOrigin))
		{
			//exclude the parallel
			if (fabs(polyEdge_1.y - polyEdge_2.y) < EPSILON)
			{
				continue;
			}
			if (IsIntersect(polyEdge_1, polyEdge_2, rayOrigin, rayEnd))
			{
				count++;
			}
		}
	}
		if (count % 2 == 1)
		{
			isInside = true;
		}
		return isInside;
}

bool SampleMethod::isValidPoint(vector<vector<glm::vec2>>& grids, float cellSize,int canvas_width,int canvas_height, glm::vec2 point, float radius)
{
	if (point.x < 0 || point.x >= canvas_width || point.y < 0 || point.y >= canvas_height)
	{
		return false;
	}
	//check neighbour cells
	int xindex = floor(point.x / cellSize);
	int yindex = floor(point.y / cellSize);

	int i0 = max(xindex - 1, 0);
	int i1 = min(xindex + 1, canvas_width - 1);

	int j0 = max(yindex - 1, 0);
	int j1 = min(yindex + 1, canvas_height - 1);



	for (int j = j0; j <= j1; j++)
	{
		for (int i = i0; i <= i1; i++)
		{
			if (grids[j][i] != glm::vec2(0.f, 0.f))
			{		
				if (glm::distance(grids[j][i], point) < radius)
				{
					return false;
				}
			}
		}
	}
	// currently not include SVG range judge
	return true;
}
//this put samples on the whole canvas, so need to check whether this is grid is inside the shape bounds
vector<point> SampleMethod::poissionDiskSampling(float radius, float k, int canvas_width, int canvas_height, glm::vec2 canvas_pos, glm::vec3 fillCol, glm::vec3 strokeCol,NSVGpath* path,float strokeWidth)
{
	int N = 2; // 2D screen
	vector<glm::vec2> pointList;
	vector<glm::vec2> activePoints;

	vector<point> colorPointList;

	//The sample cell size
	float cellSize =radius / sqrt(N);

	// Here need to know pattern width and height
	// figure number of cells within a given canvas

	int ncells_width = ceil(canvas_width / cellSize) + 1;
	int ncells_height = ceil(canvas_height / cellSize) + 1;

	turnBezierIntoPoly(path);

	//grid are identified with (x,y) int
	// each grid contain a vector pushing point inside
	vector<vector<glm::vec2>> grids(ncells_height);

	//Allocate the grid and initialize all elements to null
	for (int i = 0; i < ncells_height; i++)
	{
		for (int j = 0; j < ncells_width; j++)
		{
			glm::vec2 emptyPoint=glm::vec2(0.f, 0.f);
			grids[i].push_back(emptyPoint);
		}
	}

	int x = rand() % canvas_width;
	int y = rand() % canvas_height;

	glm::vec2 init_p0 = glm::vec2(x, y);

	insertSamplePoint(init_p0,grids,cellSize);
	pointList.push_back(init_p0);
	activePoints.push_back(init_p0);

	while (activePoints.size() > 0)
	{
		/* Pick a point 'p' from our active list */

      /* Try up to 'k' times to find a point that satisfies:
      * -- Is at a distance between r and 2r from p
      * -- Is at a distance > r from nearby points
      */
      /* If succeed in finding a point, add to grid and lists */
      /* Otherwise, remove point 'p' from the active list */

		int randomIndex = rand() % activePoints.size();
		glm::vec2 p = activePoints.at(randomIndex);

		unsigned int seed = 0;
		static std::default_random_engine e(seed);

		bool found = false;
		for (int tries = 0; tries < k; tries++)
		{
			// found a valid point, set flag to true
			//create a new point
			//Pick a random angle 
			float theta = rand() % 360;

			// Pick a random radius between r and 2r

			std::uniform_real_distribution<double> u(radius, 2 * radius);
			float new_radius = u(e);

			float p_newx = p.x + new_radius * cos(glm::radians(theta));
			float p_newy = p.y + new_radius * sin(glm::radians(theta));

			glm::vec2 newPoint = glm::vec2(p_newx, p_newy);
			glm::vec3 pointColor;

			if (!isValidPoint(grids,cellSize,canvas_width,canvas_height,newPoint,radius))
			{
				continue;
			}

			pointColor = (fillCol == glm::vec3(0, 0, 0)) ? strokeCol : fillCol;
			point res_point;
			res_point.color = pointColor;
			res_point.position = newPoint+canvas_pos;
			if (pointInsidePoly(res_point.position, strokeWidth))
			{
				//std::cout << "Debug: inside poly" << std::endl;
				colorPointList.push_back(res_point);
			}

			//pointList.push_back(newPoint);
			insertSamplePoint(newPoint,grids,cellSize);
			activePoints.push_back(newPoint);
			found = true;
			break;
		}
		/* If no point was found after k tries, remove p */
		if (!found)
		{
			activePoints.erase(activePoints.begin()+randomIndex);
		}
	 }


	return colorPointList;

}