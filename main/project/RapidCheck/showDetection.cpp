#include "main.h"
using namespace cv;

/**
	Show similarities of given target paris

	@param app frame reader with basic parameters set
*/
void showDetection(App app)
{
	
	Mat frame, targetImage;
	Target target;
	bool hasTarget = false;

	// create a tracker object
	std::vector<Ptr<Tracker> > trackers;
	for (int i = 0; i < MAX_TRACKER_NUMS; ++i)
		trackers.push_back(Tracker::create("KCF"));
	std::vector<Rect> found;
	std::vector<Rect2d> found_filtered;

	// set input video
	VideoCapture cap(VIDEOFILE);
	// cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
	// cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
	cap >> frame;
	
	// Histogram setting
	// Using 50 bins for hue and 60 for saturation
	int h_bins = NUM_OF_HUE_BINS; int s_bins = NUM_OF_SAT_BINS;
	int histSize[] = { h_bins, s_bins };

	// hue varies from 0 to 179, saturation from 0 to 255
	float h_ranges[] = { 0, 180 };
	float s_ranges[] = { 0, 256 };

	const float* ranges[] = { h_ranges, s_ranges };

	// Use the o-th and 1-st channels
	int channels[] = { 0, 1 };
	int frameCnt = 0;
	while (frameCnt < MAX_FRAMES)
	{
		int frameNumber = START_FRAME_NUM + frameCnt*FRAME_STEP;
		cap.set(CV_CAP_PROP_POS_FRAMES, frameNumber);
		// get frame from the video
		cap >> frame;
		Mat clone = frame.clone();
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
				Rect2d &r = found_filtered[i];
			
				r.x += cvRound(r.width*0.1);
				r.width = cvRound(r.width*0.8);
				r.y += cvRound(r.height*0.07);
				r.height = cvRound(r.height*0.8);
				rectangle(frame, r, Scalar(0, 255, 0), 3);
			}
		}
		
		// draw frame
		imshow("tracker", frame);
		frameCnt++;

		//quit on ESC button
		int key = waitKey(3);
		if (key == 27) break;
	}
}