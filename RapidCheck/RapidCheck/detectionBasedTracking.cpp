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
	args.hit_threshold = 0.9;
	args.hit_threshold_auto = false;
	args.gr_threshold = 6;
	App app(args);
	Mat frame, targetImage;
	Target target;
	bool hasTarget = false;

	//app.run();
	
	vector<Frame> frames;
	vector<Point> traces;

	
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

	int frameNum = 1;
	while (true)
	{
		
		// set frame number
		cap.set(CV_CAP_PROP_POS_FRAMES, frameNum);

		// get frame
		Mat frame, frame_edge, frame_contour;
	
		cap >> frame;
		cvtColor(frame, frame_edge, COLOR_BGR2GRAY);
		cvtColor(frame, frame_contour, COLOR_BGR2GRAY);
		vector<Rect> & pedestrians = frames[frameNum].getPedestrians(), & prevPedestrians = frames[frameNum-1].getPedestrians();
		
		// print rect informations
		cout << "frame Num : " << frameNum << endl;
		cout << " - previous frame pedestrians : ";
		for (int i = 0; i < prevPedestrians.size(); i++)
		{
			Rect rect = prevPedestrians[i];
			Point center(rect.x + rect.width / 2, rect.y + rect.height / 2);
			traces.push_back(center);
			printf("(%d,%d,%d,%d)\t", rect.x, rect.y, rect.width, rect.height);
		}
		cout << endl;
		cout << " - current frame pedestrians : ";
		for (int i = 0; i < pedestrians.size(); i++)
		{
			Rect rect = pedestrians[i];
			Point center(rect.x + rect.width / 2, rect.y + rect.height / 2);
			traces.push_back(center);
			printf("(%d,%d,%d,%d)\t", rect.x, rect.y, rect.width, rect.height);
		}
		cout << endl;
		for (int i = 0; i < pedestrians.size(); i++)
		{
			Rect cur = pedestrians[i];
			for (int j = 0; j < prevPedestrians.size(); j++)
			{
				Rect prev = prevPedestrians[j];
				int dx = cur.x - prev.x, dy = cur.y - prev.y, distSquare = dx*dx + dy*dy;
				printf("(%d,%d):%d\t", dx, dy, distSquare);
			}
			cout << endl;
		}
		
		for (int i = 0; i < pedestrians.size(); i++)
		{
			rectangle(frame, pedestrians[i], Scalar(0, 255, 0), 2, 1);
		}
		
		// get edge
		Mat temp;
		/*
		for (int i = 0; i < pedestrians.size(); i++)
		{
			Rect& rect = pedestrians[i];
			Canny(frame_edge(rect), frame_edge(rect), 100, 200, 3);

			vector<vector<Point> > contours;
			findContours(frame_edge(rect), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_KCOS);
			
			
			int maxLen = 0, maxIdx = -1;
			for (int i = 0; i < contours.size(); i++) {
				int curLen = contours[i].size();
				if (maxLen < curLen) {
					maxLen = curLen;
					maxIdx = i;
				}
			}
			cout << "maxLen : " << maxLen << endl;
			
			vector<Point> convex;
			for (int i = 0; i < contours.size(); i++)
				for (int j = 0; contours[i].size() > 30 && j < contours[i].size(); j++)
					convex.push_back(contours[i][j]);
				
			convexHull(convex, convex);
			// drawContours(frame_contour(rect), contours, -1, Scalar(255, 255, 255), 1);
			polylines(frame_contour(rect), convex, true, Scalar(255, 255, 255), 2);
			
		}
		
		imshow("edge", frame_edge);
		imshow("contour", frame_contour);
		*/
		// draw traces
		for (int i = 0; i < traces.size(); i++) {
			circle(frame, traces[i], 1, Scalar(255, 255, 255), 1);
		}


		imshow("result", frame);
		int key = waitKey(0);
		if (key == 27) break;
		if (key == (int)('n'))
		{
			cout << frameNum << endl;
			frameNum = min(frameNum + 1, MAX_FRAMES - 1);
		}
		else if (key == (int)('b'))
		{
			frameNum = max(frameNum - 1, 1);
		}
	}

}