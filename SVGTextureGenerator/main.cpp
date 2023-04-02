#include "SVGContext.h"
#include "image_data.h"
#include "cluster.h"
#include "sampleMethod.h"
#include <fstream>
#include <string>

enum SVGTag
{
	Polygon,
	Path,
	Cylinder,
	Rectangle
};

void generateSVGFile(const char* path, float width, float height, vector<vector<glm::vec2>>& shapePoints,vector<vector<glm::vec2>>&pathPoints)
{
	std::ofstream file(path);
	std::string headerStr = "<?xml version=\"1.0\" standalone=\"no\"?> \n";

	std::string widthStr ="\""+ std::to_string((int)width)+"pt\"";
	std::string heightStr = "\"" + std::to_string((int)height) + "pt\"";

	headerStr += "<svg width=" + widthStr + " height= " + heightStr +" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">\n";
	for (int i = 0; i < shapePoints.size(); i++)
	{
		for (int j = 0; j < shapePoints[i].size(); j++)
		{
			glm::vec2 samplePoint = shapePoints[i][j];
			string point_x = "\"" + std::to_string(samplePoint.x) + "\"";
			string point_y=  "\"" + std::to_string(samplePoint.y) + "\"";
			headerStr+= "<circle cx="+point_x +" cy=" + point_y + " r=\"0.1\" stroke=\"black\" fill=\"red\" stroke-width=\"0.1\"/> \n";
		}
	}
	headerStr +="</svg>\n";
  //  cout << "Debug helper" << endl;
	file << headerStr;
	file.close();
}

//can put it into sample method class
vector<vector<glm::vec2>> sampleShape(NSVGshape* shape)
{
	SampleMethod* instance = SampleMethod::getInstance();
	vector<vector<glm::vec2>> shapeSamplePoints;
	while (shape && shape->next != nullptr)
	{

		//This svg is about shapes
		glm::vec2 leftCorner = glm::vec2(shape->bounds[0], shape->bounds[1]);
		glm::vec2 rightCorner = glm::vec2(shape->bounds[2], shape->bounds[3]);

		int width = rightCorner.x - leftCorner.x;
		int height = rightCorner.y - leftCorner.y;

		vector<glm::vec2> samplePoints = instance->poissionDiskSampling(2, 6, width, height);
		shapeSamplePoints.push_back(samplePoints);
		shape =shape->next;
	}
	return shapeSamplePoints;
}

vector<vector<glm::vec2>> samplePath(NSVGpath* path)
{
	SampleMethod* instance = SampleMethod::getInstance();
	vector<vector<glm::vec2>> pathSamplePoints;
	while (path && path->next != nullptr)
	{

		//This svg is about shapes
		glm::vec2 leftCorner = glm::vec2(path->bounds[0], path->bounds[1]);
		glm::vec2 rightCorner = glm::vec2(path->bounds[2], path->bounds[3]);

		int width = rightCorner.x - leftCorner.x;
		int height = rightCorner.y - leftCorner.y;

		vector<glm::vec2> samplePoints = instance->poissionDiskSampling(5, 4, width, height);
		pathSamplePoints.push_back(samplePoints);
		path = path->next;
	}
	return  pathSamplePoints;
}

void testFunction()
{
	SVGContext context;
	//string loadFilePath = "D:\\CIS660\\CIS660-VSProj\\image\\testSave.svg";
	//string loadFilePath = "D:\\CIS660\\CIS660-VSProj\\image\\pattern.svg";
	string loadFilePath = "D:\\CIS660\\CIS660-VSProj\\image\\testPic.svg";
    context.loadSVGFromFile(loadFilePath.c_str(),10);
	NSVGimage* readImg = context.getSVGImage();

	float width = readImg->width;
	float height = readImg->height;
	SampleMethod* instance = SampleMethod::getInstance();
	//didn't check if all points is inside the shape
	NSVGshape* imgShape = readImg->shapes;
	
	NSVGpath* imgPath = readImg->shapes->paths;

	vector<vector<glm::vec2>> shapePoints;
	vector<vector<glm::vec2>> pathPoints;

	if (imgShape->next != nullptr)
	{
		while (imgShape && imgShape->next != nullptr)
		{

			//This svg is about shapes
			glm::vec2 leftCorner = glm::vec2(imgShape->bounds[0], imgShape->bounds[1]);
			glm::vec2 rightCorner = glm::vec2(imgShape->bounds[2], imgShape->bounds[3]);

			int width = rightCorner.x - leftCorner.x;
			int height = rightCorner.y - leftCorner.y;

			vector<glm::vec2> samplePoints = instance->poissionDiskSampling(2, 6, width, height);
			shapePoints.push_back(samplePoints);
			imgShape = imgShape->next;
		}
	}

	if (imgPath->next != nullptr)
	{
		while (imgPath && imgPath->next != nullptr)
		{

			//This svg is about shapes
			glm::vec2 leftCorner = glm::vec2(imgPath->bounds[0], imgPath->bounds[1]);
			glm::vec2 rightCorner = glm::vec2(imgPath->bounds[2], imgPath->bounds[3]);

			int width = rightCorner.x - leftCorner.x;
			int height = rightCorner.y - leftCorner.y;

			vector<glm::vec2> samplePoints = instance->poissionDiskSampling(5, 4, width, height);
			pathPoints.push_back(samplePoints);
			imgPath = imgPath->next;
		}
	}

	string saveFilePath = "D:\\CIS660\\CIS660-VSProj\\image\\Save.svg";
	generateSVGFile(saveFilePath.c_str(),width,height,shapePoints,pathPoints);
	cout << "Debug helper" << endl;
}


int main()
{
	testFunction();
	return 1;
}