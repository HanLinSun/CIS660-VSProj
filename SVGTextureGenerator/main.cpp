#include "SVGContext.h"
#include "image_data.h"
#include "cluster.h"
#include "sampleMethod.h"





void testFunction()
{
	image_data overall_image;

	SVGContext context;
	//string loadFilePath = "D:\\CIS660\\CIS660-VSProj\\image\\testSave.svg";
	//string loadFilePath = "D:\\CIS660\\CIS660-VSProj\\image\\pattern.svg";
	string loadFilePath = "D:\\cis660Final\\CIS660_REAL_FINAL\\CIS660-VSProj\\image\\testSave.svg";
	context.loadSVGFromFile(loadFilePath.c_str(), 10);
	NSVGimage* readImg = context.getSVGImage();

	float width = readImg->width;
	float height = readImg->height;
	SampleMethod* instance = SampleMethod::getInstance();
	//didn't check if all points is inside the shape
	NSVGshape* imgShape = readImg->shapes;

	int clusterID = 0;
	int sample_count = 0;
	while (imgShape != nullptr)
	{
		glm::vec2 leftCorner = glm::vec2(imgShape->bounds[0], imgShape->bounds[1]);
		glm::vec2 rightCorner = glm::vec2(imgShape->bounds[2], imgShape->bounds[3]);

		int width = rightCorner.x - leftCorner.x;
		int height = rightCorner.y - leftCorner.y;

		vector<glm::vec2> samplePoints = instance->poissionDiskSampling(5, 4, width, height);

		std::unique_ptr<cluster> new_cluster = std::make_unique<cluster>();
		
		for (auto& m_sample : samplePoints) 
		{
			std::unique_ptr<sample> new_sample = std::make_unique<sample>();
			new_sample->cluster_ID = clusterID;
			new_sample->col = vec3(1, 1, 1);
			new_sample->position = m_sample;


			new_cluster->sample_list.push_back(new_sample->position);
			new_cluster->set_id(clusterID);

			//adding sample to overall image
			overall_image.sample_data[sample_count] = std::move(new_sample);
			sample_count += 1;

		}
		overall_image.cluster_data[clusterID] = std::move(new_cluster);

		imgShape = imgShape->next;
		clusterID += 1;
	}

	overall_image.calculate_neighbor(0);

	overall_image.init_output_image();



	cout << "Debug helper" << endl;

}


int main()
{
	testFunction();
	return 1;
}