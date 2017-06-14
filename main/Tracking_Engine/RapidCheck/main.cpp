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
	filepath = "C:/videos/tracking.mp4";
	videoId = 3;
	numOfFrames = 1000;
	startFrameNum = 0;
	frameStep = 3;
	endFrameNum = startFrameNum + numOfFrames * frameStep;
}


int main(int argc, char ** argv)
{
	initRCVariables();
	int operationNum = 6;
	
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
			compareSimilarity();
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
	}
	return 0;
}