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
#define DETECTION_PERIOD 10
#define MAX_TRACKER_NUMS 10

#define MARGIN 50

using namespace cv;

int main(int argc, char ** argv)
{
	// declares all required variables
	Args args;
	App app(args);
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
				putText(frame, std::to_string(i), existingTarget.centerPositions.back(), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 2);
			}
		}
		
		
		// draw frame
		imshow("tracker", frame);
		frameCnt++;

		//quit on ESC button
		int key = waitKey(3);
		if (key == 27) break;

		// puase on p key pressed
		if (key == (int)('p'))
		{
		
			// target index
			int idx = 0;
			key = waitKey(3);
			while (key != (int)('p'))
			{

				int width = 0, height = 0;
				// calculate width and height
				for (int i = 0; i < currentFrameTargets.targets.size(); i++)
				{
					Target& currentFrameTarget = currentFrameTargets.targets[i];
					width += currentFrameTarget.rect.width + MARGIN;
					height = max(height, currentFrameTarget.rect.height);
				}
				Mat targets(height + 2*MARGIN, width + MARGIN, CV_8UC3);
				
				// draw targets
				int start = MARGIN;
				for (int i = 0; i < currentFrameTargets.targets.size(); i++)
				{
					Target& currentFrameTarget = currentFrameTargets.targets[i];
					Rect rect = currentFrameTarget.rect;
					Mat roi = clone(rect);
					roi.copyTo(targets(Rect(start, MARGIN, rect.width, rect.height)));
					
					if (hasTarget) {
						// calculate similarity
						double sim = compareHist(currentFrameTarget.hist, target.hist, 0);
						//cout << sim << endl;
						putText(targets, to_string(sim*100).substr(0,5), Point (start, MARGIN), CV_FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255, 0, 0), 2);
					}
					start += rect.width + MARGIN;
				}
				imshow("Current Frame targets", targets);

				if (hasTarget) {
					imshow("Existing target", targetImage);
				}
		
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
				case (int)('t') :
					idx = (idx + 1) % currentFrameTargets.targets.size();
					target = currentFrameTargets.targets[idx];
					targetImage = clone(target.rect);
					hasTarget = true;
					break;
		
				case (int)('c') :
					// double similarity0 = compareHist(left.hist, right.hist, 0);
					// double similarity1 = compareHist(left.hist, right.hist, 1);
					// double similarity2 = compareHist(left.hist, right.hist, 2);
					// double similarity3 = compareHist(left.hist, right.hist, 3);
					// cout << " - calculate similarities using 4 methods" << endl;
					// printf("s0:%.2lf s1:%.2lf s2:%.2lf s3:%.2lf\n", similarity0, similarity1, similarity2, similarity3);
					break;
				}

			}
		}
	}
	return 0;
}