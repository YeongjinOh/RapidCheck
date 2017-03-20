#include "tracking_utils.h"

#define MAX_FRAMES 500
#define NUM_OF_SEGMENTS (MAX_FRAMES - 1)/LOW_LEVEL_TRACKLETS
#define NUM_OF_COLORS 64
#define DEBUG false
#define start 0 // start frame number

/**
	Build tracklets of all segements and then, show trace of tracklets

	@param app frame reader with basic parameters set
*/
void showTracklet(App app)
{
	// set input video
	VideoCapture cap(VIDEOFILE);

	// random number generator
	RNG rng(0xFFFFFFFF);

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
	cap.set(CV_CAP_PROP_POS_FRAMES, start);
	for (int frameNum = start; frameNum < start + MAX_FRAMES; frameNum++) {

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

	
	// build all segments
	vector<Segment> segments;
	int frameNum = 1;
	for (int segmentNumber = 0; segmentNumber < NUM_OF_SEGMENTS; segmentNumber++, frameNum += LOW_LEVEL_TRACKLETS)
	{
		// set frame number
		cap.set(CV_CAP_PROP_POS_FRAMES, frameNum + start);
		Segment segment(frameNum + start);

		// get frame
		Mat frame;
		cap >> frame;

		// create tracklet
		vector<int> solution;

		// do not use dummy until no more solution built to optimize
		bool useDummy = false;
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
				continue;
			}
			
			// for each solution
			tracklet pedestrianTracklet;
			for (int i = 0; i < solution.size(); i++)
			{
				Frame & curFrame = frames[frameNum + i];
				Target & target = curFrame.getTarget(solution[i]);
				target.found = true;
				pedestrianTracklet.push_back(target);
			}
			segment.addTracklet(pedestrianTracklet);
			
		}
		segments.push_back(segment);
	}

	cout << "Tracklets built" << endl;

	for (int segmentNumber = 0; segmentNumber < NUM_OF_SEGMENTS; segmentNumber++)
	{
		Segment & segment = segments[segmentNumber];
		for (int frameIdx = 1; frameIdx <= LOW_LEVEL_TRACKLETS; frameIdx++)
		{
			int frameNum = LOW_LEVEL_TRACKLETS * segmentNumber + frameIdx + start;
			cap.set(CV_CAP_PROP_POS_FRAMES, frameNum);
			cap >> frame;

			vector<tracklet>& pedestrianTracklets = segment.tracklets;
			for (int pedestrianNum = 0; pedestrianNum < pedestrianTracklets.size(); pedestrianNum++)
			{
				tracklet& pedestrianTracklet = pedestrianTracklets[pedestrianNum];
				Target& currentFramePedestrian = pedestrianTracklet[frameIdx - 1];
				rectangle(frame, currentFramePedestrian.rect, RED, 2);
				// circle(frame, currentFramePedestrian.getCenterPoint(), 2, RED, 2);
			}
			imshow("tracklets", frame);

			// key handling
			int key = waitKey(10);
			if (key == 27) break;
			else if (key == (int)('r'))
			{
				segmentNumber = 0;
				break;
			}
		}
	}
}