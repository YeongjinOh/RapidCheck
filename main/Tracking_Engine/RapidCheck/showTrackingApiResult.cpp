#include "main.h"
using namespace cv;
using namespace rc;


#include <opencv2/opencv.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cstring>
#include <vector>

/**
	Show tracking result using OpenCV Tracking API
*/
void showTrackingApiResult()
{
	App app = App();
	Mat frame, targetImage;
	Target target;
	bool hasTarget = false;

	std::vector<Rect> found;
	std::vector<Rect2d> found_filtered;


	vector<Ptr<Tracker> > trackers;


	// set input video
	VideoCapture cap(filepath);
	
	cap >> frame;

	int frameCnt = 0;
	while (frameCnt < numOfFrames)
	{
		int frameNumber = startFrameNum + frameCnt * frameStep;
		cap.set(CV_CAP_PROP_POS_FRAMES, frameNumber);
		// get frame from the video
		cap >> frame;
		Mat clone = frame.clone();
		// stop the program if no more images
		if (frame.rows == 0 || frame.cols == 0)
			break;

		if (frameCnt == 0)
		{
			// reset vectors
			found.clear();
			found_filtered.clear();

			app.getHogResults(frame, found);

			size_t i, j;
			for (int i = 0; i<found.size(); i++)
			{
				Rect r = found[i];
				for (j = 0; j<found.size(); j++)
					if (j != i && (r & found[j]) == r)
						break;
				if (j == found.size())
					found_filtered.push_back(r);
			}

			for (i = 0; i<found_filtered.size(); i++)
			{
				Rect2d &r = found_filtered[i];

				r.x += cvRound(r.width*0.1);
				r.width = cvRound(r.width*0.8);
				r.y += cvRound(r.height*0.07);
				r.height = cvRound(r.height*0.8);
				rectangle(frame, r, Scalar(0, 255, 0), 3);
			}


			for (int i = 0; i < found_filtered.size(); i++)
			{
				trackers.push_back(Tracker::create("KCF"));
				trackers[i]->init(frame, found_filtered[i]);
			}
			int key = waitKey(0);
		}
		else
		{
			for (int i = 0; i < trackers.size(); i++)
			{
				trackers[i]->update(frame, found_filtered[i]);
				Rect2d &r = found_filtered[i];
				rectangle(frame, r, Scalar(0, 255, 0), 3);
			}
		}



		// draw frame
		imshow("OpenCV KCF Tracker", frame);
		frameCnt++;

		//quit on ESC button
		int key = waitKey(3);
		if (key == 27) break;
	}
}