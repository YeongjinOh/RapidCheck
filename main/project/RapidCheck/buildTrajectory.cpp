#include "tracking_utils.h"

/**
	Build trajectories of all segements and then, show trace of tracklets

	@param app frame reader with basic parameters set
*/
void buildTrajectory(App app)
{
	// set input video
	VideoCapture cap(VIDEOFILE);

	// random number generator
	RNG rng(0xFFFFFFFF);

	// initialize colors	
	vector<Scalar> colors;
	for (int i = 0; i < NUM_OF_COLORS; i++)
	{
		int icolor = (unsigned)rng;
		int minimumColor = 0;
		colors.push_back(Scalar(minimumColor + (icolor & 127), minimumColor + ((icolor >> 8) & 127), minimumColor + ((icolor >> 16) & 127)));
	}

	// build target detected frames
	vector<Frame> frames;
	detectTargets(app, cap, frames);
	cout << "Detection finished" << endl;

	// build all tracklets
	vector<Segment> segments;
	buildTracklets(frames, segments);
	cout << "Tracklets built" << endl;

	// build trajectories
	// TODO
	
}