#include <opencv2/opencv.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cstring>
#include <vector>

#include "gpu_hog.h"

#define VIDEOFILE "videos/street.avi"
#define DETECTION_PERIOD 5
#define MAX_TRACKER_NUMS 10

using namespace cv;

int main(int argc, char ** argv)
{
		// declares all required variables
	Rect2d roi;
	Mat frame;

	// create a tracker object
	std::vector<Ptr<Tracker> > trackers;
	for (int i = 0; i < MAX_TRACKER_NUMS; ++i)
		trackers.push_back(Tracker::create("KCF"));

	Args args;
	App app(args);

	std::vector<Rect> found;
	std::vector<Rect2d> found_filtered;

	// set input video
	VideoCapture cap(VIDEOFILE);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
	
	// get bounding box
	cap >> frame;
	
	// perform the tracking process
	printf("Start the tracking process, press ESC to quit.\n");

	int frameCnt = 0;

	while (true){
		// get frame from the video
		cap >> frame;
		// stop the program if no more images
		if (frame.rows == 0 || frame.cols == 0)
			break;

		// implement detection using HOG every n-th frame
		if (frameCnt % DETECTION_PERIOD == 0) {

			// reset vectors
			found.clear();
			found_filtered.clear();
			trackers.clear();
			
			app.getHogResults(frame, found);
			size_t i, j;
			for (int i = 0; i<found.size() && i < MAX_TRACKER_NUMS; i++)
			{
				Rect r = found[i];
				for (j = 0; j<found.size(); j++)
					if (j != i && (r & found[j]) == r)
						break;
				if (j == found.size())
					found_filtered.push_back(r);
			}

			for (i = 0; i<found_filtered.size() && i < MAX_TRACKER_NUMS; i++)
			{
				Rect r = found_filtered[i];
			
				r.x += cvRound(r.width*0.1);
				r.width = cvRound(r.width*0.8);
				r.y += cvRound(r.height*0.07);
				r.height = cvRound(r.height*0.8);

				Ptr<Tracker> tracker = Tracker::create("KCF");
				tracker->init(frame, r);
				trackers.push_back(tracker);
				rectangle(frame, r.tl(), r.br(), Scalar(0, 255, 0), 3);
			}
		}

		// implement tracking during remaining (n-1) frames 
		else {
			
						for (int i = 0; i < found_filtered.size() && i < trackers.size(); i++) {
				trackers[i]->update(frame, found_filtered[i]);
				rectangle(frame, found_filtered[i], Scalar(255, 0, 0), 2, 1);
			}

		}
		
		// draw frame
		imshow("tracker", frame);

		//quit on ESC button
		if (waitKey(3) == 27)break;

		frameCnt++;
	}
	return 0;
}