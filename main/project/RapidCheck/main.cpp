#include "main.h"

using namespace cv;

int main(int argc, char ** argv)
{
	// set gpu_hog
	Args args;
	args.hit_threshold = 0.9;
	args.hit_threshold_auto = false;
	args.gr_threshold = 6;
	args.scale = 1.05;
	

	
	// args.src_is_video = true;
	// args.src = VIDEOFILE;

	App app(args);

	showTracklet(app);
	//detectionBasedTracking(app);
	//detectionAndTracking(app);



	// App app(args);
	// app.run();
	
	return 0;
}