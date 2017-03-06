#include <opencv2/opencv.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cstring>
#include <vector>

#include "gpu_hog.h"
#include "targetgroup.h"

#define VIDEOFILE "videos/street.avi"
#define DETECTION_PERIOD 1
#define MAX_TRACKER_NUMS 10

using namespace cv;

int main(int argc, char ** argv)
{
	// declares all required variables
	Args args;
	App app(args);
	Mat frame;

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
	int h_bins = 50; int s_bins = 60;
	int histSize[] = { h_bins, s_bins };

	// hue varies from 0 to 179, saturation from 0 to 255
	float h_ranges[] = { 0, 180 };
	float s_ranges[] = { 0, 256 };

	const float* ranges[] = { h_ranges, s_ranges };

	// Use the o-th and 1-st channels
	int channels[] = { 0, 1 };

	TargetGroup existingTargets;
	int frameCnt = 0;
	while (frameCnt < 400) {
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

				Ptr<Tracker> tracker = Tracker::create("KCF");
				tracker->init(frame, r);
				trackers.push_back(tracker);
				rectangle(frame, r, Scalar(0, 255, 0), 3);
			}
		}

		// implement tracking during remaining (n-1) frames 
		else
		{
			for (int i = 0; i < found_filtered.size() && i < trackers.size(); i++) {
				trackers[i]->update(frame, found_filtered[i]);
				rectangle(frame, found_filtered[i], Scalar(0, 255, 0), 2, 1);
			}

		}
		
		// matching
		vector<MatND> hists;
		for (int i = 0; i < found_filtered.size(); ++i)
		{
			Mat temp;
			MatND hist;
			cvtColor(frame(found_filtered[i]), temp, COLOR_BGR2HSV);
			calcHist(&temp, 1, channels, Mat(), hist, 2, histSize, ranges, true, false);
			normalize(hist, hist, 0, 1, NORM_MINMAX, -1, Mat());
			hists.push_back(hist);
		}
		TargetGroup currentFrameTargets(found_filtered, hists);
		existingTargets.match(currentFrameTargets);
		for (int i = 0; i < existingTargets.targets.size(); ++i)
		{
			Target& existingTarget = existingTargets.targets[i];
			if (existingTarget.stillBeingTracked && existingTarget.currentMatchFoundOrNew)
			{
				int intFontFace = CV_FONT_HERSHEY_SIMPLEX;
				double dblFontScale = existingTarget.currentDiagonalSize / 60.0;
				int intFontThickness = (int)std::round(dblFontScale * 1.0);
				cv::putText(frame, std::to_string(i), existingTarget.centerPositions.back(), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 2);
			}
		}
		
		
		// draw frame
		imshow("tracker", frame);
		frameCnt++;

		//quit on ESC button
		int key = waitKey(3);
		if (key == 27) break;

		// puase on p key pressed
		if (key == (int)('p')) {

			// target index
			int idx = 0;
			key = waitKey(3);
			while (key != (int)('p')) {

				Target& currentFrameTarget = currentFrameTargets.targets[idx];
				Mat roi = clone(currentFrameTarget.rect);
				imshow("target", roi);

				key = waitKey(3);
				switch (key) {
				case 27:
					return 0;
					break;

				case (int)('n') :
					if (idx == currentFrameTargets.targets.size() - 1)
					{
						cout << "The last target" << endl;
					}
					else
					{
						cout << "idx++" << endl;
						idx++;
					}
								break;
				case (int)('b') :
					if (idx == 0)
					{
						cout << "The first target" << endl;
					}
					else
					{
						cout << "idx--" << endl;
						idx--;
					}
								break;
				}
			}
		}
	}
	return 0;
}