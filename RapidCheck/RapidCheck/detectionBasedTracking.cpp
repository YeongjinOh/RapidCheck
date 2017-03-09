#include "main.h"

#define MAX_FRAMES 100

// set input video
VideoCapture cap(VIDEOFILE);


Mat getFrame(vector<Frame>& frames, int frameNum) {
	
	Mat frame;
	cap >> frame;
	vector<Rect> & pedestrians = frames[frameNum].getPedestrians();
	for (int i = 0; i < pedestrians.size(); i++) {
		rectangle(frame, pedestrians[i], Scalar(0, 255, 0), 2, 1);
	}
	return frame;
}

void detectionBasedTracking()
{
	Args args;
	App app(args);
	Mat frame, targetImage;
	Target target;
	bool hasTarget = false;
	
	vector<Frame> frames;

	
	for (int frameNum = 0; frameNum < MAX_FRAMES; frameNum++) {
		
		// get frame from the video
		cap >> frame;
	
		// stop the program if no more images
		if (frame.rows == 0 || frame.cols == 0)
			break;

		vector<Rect> found, found_filtered;

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
			Rect &r = found_filtered[i];
			r.x += cvRound(r.width*0.1);
			r.width = cvRound(r.width*0.8);
			r.y += cvRound(r.height*0.07);
			r.height = cvRound(r.height*0.8);
		}
	
		frames.push_back(Frame(frameNum, found_filtered));
	}

	cout << "Detection finished" << endl;

	int frameNum = 0;
	while (true) {
		
		// set frame number
		cap.set(CV_CAP_PROP_POS_FRAMES, frameNum);

		// get frame
		Mat frame, frame_edge;
		cap >> frame;
		cvtColor(frame, frame_edge, COLOR_BGR2GRAY);
		vector<Rect> & pedestrians = frames[frameNum].getPedestrians();
		for (int i = 0; i < pedestrians.size(); i++) {
			rectangle(frame, pedestrians[i], Scalar(0, 255, 0), 2, 1);
		}
		
		// get edge
		Mat temp;
		for (int i = 0; i < pedestrians.size(); i++) {
			Rect& rect = pedestrians[i];
			Canny(frame_edge(rect), frame_edge(rect), 100, 200, 3);

			vector<vector<Point> > contours;
			findContours(frame_edge(rect), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
			cout << contours.size() << endl;
			drawContours(frame(rect), contours, -1, (255, 255, 255), 1);
		}
		


		imshow("edge", frame_edge);
		imshow("result", frame);
		int key = waitKey(0);
		if (key == 27) break;
		if (key == (int)('n'))
		{
			frameNum = min(frameNum + 1, MAX_FRAMES);
		}
		else if (key == (int)('b'))
		{
			frameNum = max(frameNum - 1, 0);
		}
	}

}