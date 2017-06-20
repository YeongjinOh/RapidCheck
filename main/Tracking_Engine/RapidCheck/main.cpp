#include "main.h"

namespace rc 
{
	char* filepath;
	int videoId;
	int numOfFrames;
	int startFrameNum;
	int frameStep;
	int endFrameNum;
}

using namespace rc;

void initRCVariables()
{
	int id = 0;
	char *filepaths[] = { "C:/videos/video_147254_test.avi", "C:/videos/video_439532_test.mp4", "C:/videos/video_166497_test.mp4", "C:/videos/video_716195_test.mp4" };
	filepath = filepaths[id];
	videoId = 700+id;
	startFrameNum = 0;
	frameStep = 1;
	endFrameNum = 30000;
	numOfFrames = (endFrameNum - startFrameNum) / frameStep;
}


int main(int argc, char ** argv)
{
	initRCVariables();
	int operationNum = 8;

	// assign arguments
	for (int i = 1; i < argc; i++)
	{
		if (std::string(argv[i]) == "--videoId")
		{
			videoId = atoi(argv[++i]);
			operationNum = 1;
		}
		else if (std::string(argv[i]) == "--operation")
		{
			operationNum = atoi(argv[++i]);
		}
		else if (std::string(argv[i]) == "--frameStep")
		{
			frameStep = atoi(argv[++i]);
		}
		else if (std::string(argv[i]) == "--maxFrameNum")
		{
			startFrameNum = 0;
			endFrameNum = atoi(argv[++i]);
		}
		else if (std::string(argv[i]) == "--path")
		{
			filepath = argv[++i];
		}
	}
	numOfFrames = (endFrameNum - startFrameNum) / frameStep;

	if (argc > 1)
	{
		analysisVideo();
		return 0;
	}
	
	switch (operationNum)
	{
		case 0:
			analysisVideo();
			break;
		case 1:
			buildAndShowTrajectory();
			break;
		case 2:
			showTracklet();
			break;
		case 3:
			showTrackletClusters();
			break;
		case 4:
			showDetection();
			break;
		case 5:
			showTrajectory();
			break;
		case 6:
			colorExtractor();
			break;
		case 7:
			compareSimilarity();
			break;
		case 8:
			showTrackingApiResult();
			break;
	}
	return 0;
}