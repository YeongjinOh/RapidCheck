#include "main.h"

void detectionBasedTracking()
{
	Args args;
	App app(args);
	Mat frame, targetImage;
	Target target;
	bool hasTarget = false;
	vector<Rect> found;
	vector<Rect2d> found_filtered;

	// set input video
	VideoCapture cap(VIDEOFILE);
	
	
	for (int frameNum = 0; frameNum < 400; frameNum++) {
		
		// get frame from the video
		cap >> frame;
		Mat clone = frame.clone();

		// stop the program if no more images
		if (frame.rows == 0 || frame.cols == 0)
			break;

		// reset vectors
		found.clear();
		found_filtered.clear();

		// implement hog detection
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
	
		// draw frame
		imshow("tracker", frame);
	
		//quit on ESC button
		int key = waitKey(3);
		if (key == 27) break;	
	}
}