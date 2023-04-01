#include "sampleMethod.h"
#include <stdlib.h> 
#include <random>

SampleMethod::SampleMethod() {}
SampleMethod::~SampleMethod() {}

void SampleMethod::insertSamplePoint(glm::vec2 point, vector<vector<glm::vec2>>& grids,float cellSize)
{
	int xIndex = floor(point.x / cellSize);
	int yIndex = floor(point.y / cellSize);
	//put this point into corresponding grid
	grids[xIndex][yIndex] = point;
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

	for (int i = i0; i <= i1; i++)
	{
		for (int j = j0; j <= j1; j++)
		{
			if (grids[i][j] != glm::vec2(0.f, 0.f))
			{		
				if (glm::distance(grids[i][j], point) < radius)
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
vector<glm::vec2> SampleMethod::poissionDiskSampling(float radius, float k, int canvas_width, int canvas_height)
{
	int N = 2; // 2D screen
	vector<glm::vec2> pointList;
	vector<glm::vec2> activePoints;

	//The sample cell size
	float cellSize =radius / sqrt(N);

	// Here need to know pattern width and height
	// figure number of cells within a given canvas

	int ncells_width = ceil(canvas_width / cellSize) + 1;
	int ncells_height = ceil(canvas_height / cellSize) + 1;

	//grid are identified with (x,y) int
	// each grid contain a vector pushing point inside
	vector<vector<glm::vec2>> grids(ncells_height, vector<glm::vec2>(ncells_width));

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

			if (!isValidPoint(grids,cellSize,canvas_width,canvas_height,newPoint,radius))
			{
				continue;
			}
			pointList.push_back(newPoint);
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
	return pointList;

}