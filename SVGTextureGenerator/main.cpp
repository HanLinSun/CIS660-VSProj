#include "SVGContext.h"
#include "image_data.h"
#include "cluster.h"
#include "sampleMethod.h"
#include "computeUtil.h"
#include <fstream>
#include <string>

enum SVGTag
{
	Polygon,
	Path,
	Cylinder,
	Rectangle
};

struct path_cluster
{
	cluster* cluster;
	NSVGpath* path;
};

void generateSVGFile(const char* path, float width, float height, vector<vector<point>>& shapePoints)
{
	std::ofstream file(path);
	std::string headerStr = "<?xml version=\"1.0\" standalone=\"no\"?> \n";

	std::string widthStr = "\"" + std::to_string((int)width) + "pt\"";
	std::string heightStr = "\"" + std::to_string((int)height) + "pt\"";

	headerStr += "<svg width=" + widthStr + " height= " + heightStr + " version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">\n";
	for (int i = 0; i < shapePoints.size(); i++)
	{
		for (int j = 0; j < shapePoints[i].size(); j++)
		{
			glm::vec2 samplePoint = shapePoints[i][j].position;
			string point_x = "\"" + std::to_string(samplePoint.x) + "\"";
			string point_y = "\"" + std::to_string(samplePoint.y) + "\"";
			glm::vec3 color = shapePoints[i][j].color;

			string r = std::to_string(color.x);
			string g = std::to_string(color.y);
			string b = std::to_string(color.z);

			string ColorStr = r + ',' + g + ',' + b;
			
			headerStr += "<circle cx=" + point_x + " cy=" + point_y + " r=\"0.1\" stroke=\"rgb("+ ColorStr +")\" fill=\"rgb("+ColorStr+")\" stroke-width=\"0.2\"/> \n";
		}
	}
	headerStr += "</svg>\n";
	//  cout << "Debug helper" << endl;
	file << headerStr;
	file.close();
}

void generateSVGFile(const char* path, float width, float height, vector<vector<glm::vec2>>& shapePoints)
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
			headerStr+= "<circle cx="+point_x +" cy=" + point_y + " r=\"0.1\" stroke=\"black\" fill=\"red\" stroke-width=\"0.2\"/> \n";
		}
	}
	headerStr +="</svg>\n";
  //  cout << "Debug helper" << endl;
	file << headerStr;
	file.close();
}


void testFunction()
{
	image_data overall_image;

	SVGContext context;
	//"D:\cis660Final\CIS660_REAL_FINAL\CIS660-VSProj\image\testPic.svg"
	//string loadFilePath = "D:\\cis660Final\\CIS660_REAL_FINAL\\CIS660-VSProj\\image\\testSave.svg";
	string loadFilePath = "D:\\CIS660\\CIS660-VSProj\\image\\testSave.svg";
	//string loadFilePath = "D:\\CIS660\\CIS660-VSProj\\image\\testPic.svg";
    context.loadSVGFromFile(loadFilePath.c_str(),10);
	NSVGimage* readImg = context.getSVGImage();

	float width = readImg->width;
	float height = readImg->height;
	SampleMethod* instance = SampleMethod::getInstance();
	//didn't check if all points is inside the shape
	NSVGshape* imgShape = readImg->shapes;
	
	NSVGpath* imgPath = readImg->shapes->paths;

	vector<vector<glm::vec2>> shapePoints;

	vector<vector<point>> debugshapePoints;

	vector<vector<glm::vec2>> pathPoints;

	int clusterID = 0;
	int sample_count = 0;
	for (imgShape; imgShape!=NULL; imgShape=imgShape->next)
	{
		for (imgPath=imgShape->paths; imgPath != NULL; imgPath = imgPath->next)
		{
			glm::vec2 leftCorner = glm::vec2(imgPath->bounds[0], imgPath->bounds[1]);
			glm::vec2 rightCorner = glm::vec2(imgPath->bounds[2], imgPath->bounds[3]);

			int width = rightCorner.x - leftCorner.x;
			int height = rightCorner.y - leftCorner.y;

			glm::vec3 fillCol = decodeColor(imgShape->fill.color);
			glm::vec3 strokeCol = decodeColor(imgShape->stroke.color);

			int npts = imgPath->npts;
			float* pts = imgPath->pts;

	
		

			float strokeWidth = imgShape->strokeWidth;
			vector<point> samplePoints = instance->poissionDiskSampling(2, 4, width, height, leftCorner,fillCol,strokeCol,imgPath, strokeWidth);

			debugshapePoints.push_back(samplePoints);

			std::unique_ptr<cluster> new_cluster = std::make_unique<cluster>();


			for (auto& m_sample : samplePoints)
			{
				std::unique_ptr<sample> new_sample = std::make_unique<sample>();
				new_sample->cluster_ID = clusterID;
				new_sample->col = vec3(1, 1, 1);
				new_sample->position = m_sample.position;
				new_sample->index = sample_count;


				new_cluster->sample_list.push_back(new_sample->position);
				new_cluster->set_id(clusterID);

				//adding sample to overall image
				overall_image.sample_data[sample_count] = std::move(new_sample);
				sample_count += 1;
			}
			overall_image.cluster_data[clusterID] = std::move(new_cluster);


			clusterID += 1;

		}
	}

	//overall_image.calculate_neighbor(0);
	//overall_image.init_output_image();

	////overall_image.pair_match();
	//
	//for (auto& [key, m_cluster] : overall_image.output_cluster) {
	//	shapePoints.push_back(m_cluster->sample_list);
	//}
	//
	string saveFilePath = "D:\\CIS660\\CIS660-VSProj\\image\\Save.svg";
	//generateSVGFile(saveFilePath.c_str(), overall_image.desired_width, overall_image.desired_hight, shapePoints);

	//generateSVGFile(saveFilePath.c_str(), overall_image.desired_width, overall_image.desired_hight, debugshapePoints);

	generateSVGFile(saveFilePath.c_str(), width,height, debugshapePoints);

	cout << "Debug helper" << endl;

}





int main()
{
	testFunction();
	return 1;
}