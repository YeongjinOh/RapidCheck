#include "tracking_utils.h"

using namespace cv;

void showTrajectory ()
{
	VideoCapture cap(VIDEOFILE);
	vector<Frame> frames, framePedestrians, frameCars;
	readTargets(cap, framePedestrians, frameCars);
	
	// read trajectories from database
	vector<RCTrajectory> trajectoryPedestrians, trajectoryCars;
	readTrajectories(trajectoryPedestrians, CLASS_ID_PEDESTRIAN);
	readTrajectories(trajectoryCars, CLASS_ID_CAR);

	showTrajectory(framePedestrians, frameCars, trajectoryPedestrians, trajectoryCars);
}
