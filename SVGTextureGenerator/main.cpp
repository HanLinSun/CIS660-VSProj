#include "SVGContext.h"
#include "image_data.h"
#include "cluster.h"
#include "sampleMethod.h"



void testFunction()
{

	SVGContext context;
	string loadFilePath = "D:\\CIS660\\CIS660-vsproj\\SVGReader\\image\\testPic.svg";
    context.loadSVGFromFile(loadFilePath.c_str(),10);
	NSVGimage* readImg = context.getSVGImage();

	float width = readImg->width;
	float height = readImg->height;
	SampleMethod* instance = SampleMethod::getInstance();
	//didn't check if all points is inside the shape

	vector<glm::vec2> samplePoints= instance->poissionDiskSampling(5, 4, width, height);

	cout << "Debug helper" << endl;

}


int main()
{
	testFunction();
	return 1;
}