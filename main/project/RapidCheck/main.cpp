#include "main.h"

int main(int argc, char ** argv)
{
	// set gpu_hog
	Args args;
	args.hit_threshold = 0.9;
	args.hit_threshold_auto = false;
	args.gr_threshold = 6;
	args.scale = 1.05;
	App app(args);

	int operationNum = 1;
	switch (operationNum)
	{
		case 0:
			compareSimilarity(app);
			break;
		case 1:
			buildTrajectory(app);
			break;
		case 2:
			showTracklet(app);
			break;
		case 3:
			showTrackletClusters(app);
			break;
		case 4:
			detectionAndTracking(app);
			break;
		case 5:
			showTrajectory();
			break;
	}
	return 0;
}