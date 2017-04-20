#include "tracking_utils.h"
using namespace cv;
/**
	Show how to build tracklet from given detection results

	@param app frame reader with basic parameters set
*/
void detectionBasedTracking(App app)
{
	// set input video
	VideoCapture cap(VIDEOFILE);

	// random number generator
	RNG rng(0xFFFFF0FF);
	
	Mat frame, targetImage;
	Target target;
	bool hasTarget = false;

	// initialize colors	
	vector<Scalar> colors;
	for (int i = 0; i < NUM_OF_COLORS; i++) 
	{
		int icolor = (unsigned)rng;
		int minimumColor = 0;
		colors.push_back(Scalar(minimumColor + (icolor & 127), minimumColor + ((icolor >> 8) & 127), minimumColor + ((icolor >> 16) & 127)));
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
	cap.set(CV_CAP_PROP_POS_FRAMES, START_FRAME_NUM);
	for (int frameNum = START_FRAME_NUM; frameNum < START_FRAME_NUM + MAX_FRAMES; frameNum++) {
		
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
	
	int frameNum = 1, objectId;
	while (true)
	{
		printf("\n\nFrame #%d", frameNum);

		// set frame number
		cap.set(CV_CAP_PROP_POS_FRAMES, frameNum + START_FRAME_NUM + LOW_LEVEL_TRACKLETS - 1);

		// get frame
		Mat frame;
		cap >> frame;

		// draw detection responses of the first frame in this segment with green rectangle
		vector<Rect> & pedestrians = frames[frameNum + LOW_LEVEL_TRACKLETS - 1].getPedestrians();
		// &prevPedestrians = frames[frameNum + LOW_LEVEL_TRACKLETS - 1 - 1].getPedestrians();
		//for (int i = 0; i < pedestrians.size(); i++)
		//	rectangle(frame, pedestrians[i], GREEN, 2, 1);

		// create tracklet
		vector<int> solution;
		vector<Mat> segment; // set of k frames
		for (int i = 0; i < LOW_LEVEL_TRACKLETS; i++)
		{
			// set frame number
			cap.set(CV_CAP_PROP_POS_FRAMES, frameNum + START_FRAME_NUM + i);
			// get frame
			Mat cluster;
			cap >> cluster;
			segment.push_back(cluster);

			// draw detection responses with white rectangle in each cluster
			vector<Rect>& pedestrians = frames[frameNum + i].getPedestrians();
			for (int j = 0; j < pedestrians.size(); j++)
			{
				Rect rect = pedestrians[j];
				rectangle(cluster, Rect(rect.x-10, rect.y-10, rect.width+20, rect.height+20), WHITE, 2);
			}

			vector<Target>& targets = frames[frameNum + i].getTargets();
			for (int j = 0; j < targets.size(); j++)
			{
				// reset found flag
				targets[j].found = false;
				// putText(cluster, std::to_string(j), targets[j].getCenterPoint(), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 3);
			}
		}

		// do not use dummy until no more solution built to optimize
		bool useDummy = false;
		objectId = 0;
		while (true)
		{
			double costMin = INFINITY;
			solution.clear();
			
			// build solution
			getTracklet(solution, vector<int>(), vector<Target>(), frames, frameNum, costMin, useDummy);
			
			// if no more solution
			if (solution.size() < LOW_LEVEL_TRACKLETS)
			{
				if (useDummy)
					break;
				useDummy = true;
				printf("\nSolutions from dummy nodes reconstruction");
				continue;
			}
			cout << endl;

			if (DEBUG) 
				printf("cost:%f\n", costMin);
			printf("object %d : ", objectId);

			// for each solution
			for (int i = 0; i < solution.size(); i++)
			{
				Frame & curFrame = frames[frameNum + i];
				// draw center points
				Target & target = curFrame.getTarget(solution[i]);
				circle(frame, target.getCenterPoint(), 2, Scalar(255,255,255), 2);
				circle(frame, target.getCenterPoint(), 1, colors[objectId], 2);

				// draw found object in each frame
				 rectangle(segment[i], curFrame.getPedestrian(solution[i]), colors[objectId], 4, 1);
				 putText(segment[i], std::to_string(objectId), target.getCenterPoint() - Point(10,10+target.rect.height/2), CV_FONT_HERSHEY_SIMPLEX, 1, colors[objectId], 3);
				target.found = true;

				printf("%d ", solution[i]);
			}
			objectId++;
		}
		
		// show all clusters
		for (int i = 0; i < LOW_LEVEL_TRACKLETS; i++) 
		{
			resize(segment[i], segment[i], Size(400, 300));
			imshow("Frame #" + to_string(i+1), segment[i]);
		}

		// show result trace
		imshow("tracklet", frame);

		// key handling
		int key = waitKey(0);
		if (key == 27) break;
		if (key == (int)('m'))
			frameNum = min(frameNum + LOW_LEVEL_TRACKLETS, MAX_FRAMES - LOW_LEVEL_TRACKLETS);
		else if (key == (int)('v'))
			frameNum = max(frameNum - LOW_LEVEL_TRACKLETS, 1);
		else if (key == (int)('n'))
			frameNum = min(frameNum + 1, MAX_FRAMES - 1);
		else if (key == (int)('b'))
			frameNum = max(frameNum - 1, 1);
		else if (key == (int)('r'))
			frameNum = 1;
	}
}