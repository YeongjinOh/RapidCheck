#include "main.h"

#define MAX_FRAMES 60
#define MIXTURE_CONSTANT 0.1
#define LOW_LEVEL_TRACKLETS 6
#define CONTINUOUS_MOTION_COST_THRE 100
#define NUM_OF_COLORS 64
#define DEBUG false

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

double getNormValueFromVector (Point p) {
	return sqrt(p.x*p.x + p.y*p.y);
}
double costMin = INFINITY;

void getTracklet(vector<int>& solution, vector<int>& selectedIndices, vector<Target>& selectedTargets, vector<Frame>& frames, int frameNumber, bool useDummy = false)
{
	// (n+1)-th frame
	int n = selectedTargets.size();

	// base case) compute cost
	if (n >= LOW_LEVEL_TRACKLETS) {
		double costAppearance = 0.0, costMotion = 0.0;
		for (int i = 0; i < n; i++) 
		{
			for (int j = 0; j < i; j++) 
			{
				costAppearance += compareHist(selectedTargets[i].hist, selectedTargets[j].hist, 0);
			}
			for (int j = 1; j < n - 1; j++) 
			{
				Point motionErrVector = selectedTargets[i].getCenterPoint() - selectedTargets[j].getCenterPoint() - (i - j)*(selectedTargets[j + 1].getCenterPoint() - selectedTargets[j].getCenterPoint());
				double motionCost = getNormValueFromVector(motionErrVector);
				costMotion += motionCost;
			}
		}
		double cost = costAppearance + MIXTURE_CONSTANT * costMotion;

		if (DEBUG)
		{
			for (int i = 0; i < selectedIndices.size(); i++)
			{
				printf("%d ", selectedIndices[i]);
			}
			printf("appearance : %f, motion: %f cost: %f\n", costAppearance, costMotion, cost);
		}
		
		if (costMin > cost)
		{
			costMin = cost;
			solution.assign(selectedIndices.begin(), selectedIndices.end());
			
		}
		return;
	}

	Frame& frame = frames[frameNumber];
	vector<Target>& targets= frame.getTargets();
	double cost;
	for (int i = 0; i < targets.size(); i++) 
	{
		Target& curTarget = targets[i];
		if (curTarget.found)
			continue;

		// branch cutting using simple position comparison
		if (!useDummy && selectedTargets.size() > 0)
		{	
			Target& prevTarget = selectedTargets.back();
			Point motionErrVector = curTarget.getCenterPoint() - prevTarget.getCenterPoint();
			double motionCost = getNormValueFromVector(motionErrVector);
			if (motionCost > CONTINUOUS_MOTION_COST_THRE)
				continue;
		}
		selectedTargets.push_back(targets[i]);
		selectedIndices.push_back(i);
		getTracklet(solution, selectedIndices, selectedTargets, frames, frameNumber + 1);
		selectedIndices.pop_back();
		selectedTargets.pop_back();
	}
}

void detectionBasedTracking()
{

	// set gpu_hog
	Args args;
	args.hit_threshold = 0.9;
	args.hit_threshold_auto = false;
	args.gr_threshold = 6;
	App app(args);
	Mat frame, targetImage;
	Target target;
	bool hasTarget = false;

	// initialize colors
	RNG rng(0xFFFFFFFF);
	vector<Scalar> colors;
	for (int i = 0; i < NUM_OF_COLORS; i++) 
	{
		int icolor = (unsigned)rng;
		colors.push_back(Scalar(icolor & 255, (icolor >> 8) & 255, (icolor >> 16) & 255));
	}
		


	// initialize variables for histogram
	// Using 50 bins for hue and 60 for saturation
	int h_bins = 50; int s_bins = 60;
	int histSize[] = { h_bins, s_bins };
	// hue varies from 0 to 179, saturation from 0 to 255
	float h_ranges[] = { 0, 180 };
	float s_ranges[] = { 0, 256 };
	const float* ranges[] = { h_ranges, s_ranges };
	// Use the o-th and 1-st channels
	int channels[] = { 0, 1 };
	
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

		// create histograms
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
	
		frames.push_back(Frame(frameNum, found_filtered, hists));
	}

	cout << "Detection finished" << endl;

	int frameNum = 1, objectId = 0;;
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
		vector<Target> & curFrameTargets = frames[frameNum].getTargets(), prevFrameTargets = frames[frameNum - 1].getTargets();
		
		/*
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
			Rect curRect = pedestrians[i];
			MatND curHist = curFrameTargets[i].hist;
			for (int j = 0; j < prevPedestrians.size(); j++)
			{
				Rect prevRect = prevPedestrians[j];
				MatND prevHist = prevFrameTargets[j].hist;
				int dx = curRect.x - prevRect.x, dy = curRect.y - prevRect.y, distSquare = dx*dx + dy*dy;
				double similarity = compareHist(curHist, prevHist, 0);
				printf("(%d,%d):%d\t%.2lf\t", dx, dy, distSquare, similarity);
			}
			cout << endl;
		}
		*/
		for (int i = 0; i < pedestrians.size(); i++)
			rectangle(frame, pedestrians[i], Scalar(0, 255, 0), 2, 1);

		
		// create tracklet
		vector<int> solution;
		vector<Mat> segment; // set of k frames
		for (int i = 0; i < LOW_LEVEL_TRACKLETS; i++)
		{
			// set frame number
			cap.set(CV_CAP_PROP_POS_FRAMES, frameNum + i);
			// get frame
			Mat low_level_frame;
			cap >> low_level_frame;
			segment.push_back(low_level_frame);
		}

		
		while (true)
		{
			costMin = INFINITY;
			solution.clear();
			getTracklet(solution, vector<int>(), vector<Target>(), frames, frameNum);
			
			if (solution.size() < LOW_LEVEL_TRACKLETS)
				break;

			if (DEBUG)
				printf("cost:%f\n", costMin);

			for (int i = 0; i < solution.size(); i++)
			{
				Frame & curFrame = frames[frameNum + i];
				// draw center points
				Target & target = curFrame.getTarget(solution[i]);
				circle(frame, target.getCenterPoint(), 2, Scalar(0, 0, 255), 1);

				// draw found object in each frame
				rectangle(segment[i], curFrame.getPedestrian(solution[i]), colors[objectId], 4, 1);
				target.found = true;

				// print solution
				if (DEBUG) 
					cout << solution[i] << endl;
				
			}
			
			objectId++;
		} 
		
		
		for (int i = 0; i < LOW_LEVEL_TRACKLETS; i++) 
		{

			resize(segment[i], segment[i], Size(400, 300));
			imshow("result" + to_string(i), segment[i]);
		}
		
		imshow("result", frame);


		int key = waitKey(0);
		if (key == 27) break;
		if (key == (int)('m'))
		{
			cout << frameNum << endl;
			frameNum = min(frameNum + LOW_LEVEL_TRACKLETS, MAX_FRAMES - LOW_LEVEL_TRACKLETS);
		}
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