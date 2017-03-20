#include "main.h"

#define MAX_FRAMES 120
#define MIXTURE_CONSTANT 0.1
#define LOW_LEVEL_TRACKLETS 6
#define CONTINUOUS_MOTION_COST_THRE 30
#define NUM_OF_COLORS 64
#define DEBUG false

// set input video
VideoCapture cap(VIDEOFILE);

// random number generator
RNG rng(0xFFFFFFFF);

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

	if (DEBUG && useDummy) {
		printf("#%d use dummy called : ",frameNumber);
		for (int i = 0; i < selectedIndices.size(); i++)
			printf("%d ", selectedIndices[i]);
		printf("\n");
	}


	// (n+1)-th frame
	int n = selectedTargets.size();

	// base case
	if (n == LOW_LEVEL_TRACKLETS) {
		
		
		// handle outlier

		// 1. minimum number of outlier
		// 2. minimum of maxDist
		double outlierDistThres = 50.0;
		int minCntOutlier = LOW_LEVEL_TRACKLETS;
		double minMaxDist = INFINITY;
		int inlierIdx1 = -1, inlierIdx2 = -1;

		if (DEBUG)
		{
			printf("selectedIndices : ");
			for (int i = 0; i < n; i++) {
				printf("%d ", selectedIndices[i]);
			}
			printf("\n");
		}

		vector<int> inlierCandidates;
		for (int i = 0; i < n; i++)
			if (selectedIndices[i] != -1)
				inlierCandidates.push_back(i);

		// at least 4 inliers needed
		if (inlierCandidates.size() < 4)
			return;

		vector<int> minOutliers, outliers;
		for (int i = 1; i < inlierCandidates.size(); i++)
		{
			for (int j = 0; j < i; j++)
			{
				int idx1 = inlierCandidates[i], idx2 = inlierCandidates[j];
				Point p1 = selectedTargets[idx1].getCenterPoint(), p2 = selectedTargets[idx2].getCenterPoint();
				double x1 = p1.x, y1 = p1.y, x2 = p2.x, y2 = p2.y;
				// predict cluster center
				double x0 = x2 + (x1 - x2) * ((LOW_LEVEL_TRACKLETS - 1) - 2 * idx2) / (2 * (idx1 - idx2));
				double y0 = y2 + (y1 - y2) * ((LOW_LEVEL_TRACKLETS - 1) - 2 * idx2) / (2 * (idx1 - idx2));
				double maxDist = 0;
				// calculate maximal distance from cluster p0 to point pk
				int cntOutlier = 0;
				outliers.clear();
				for (int k = 0; k < inlierCandidates.size(); k++)
				{
					int idx0 = inlierCandidates[k];
					Point p_k = selectedTargets[idx0].getCenterPoint();
					double x_k = p_k.x, y_k = p_k.y;
					double dist = sqrt((x0 - x_k)*(x0 - x_k) + (y0 - y_k)*(y0 - y_k));

					if (dist > outlierDistThres)
					{
						outliers.push_back(idx0);
						cntOutlier++;
					}

					maxDist = max(maxDist, dist);
				}
				if (minCntOutlier > cntOutlier || (minCntOutlier == cntOutlier && minMaxDist > maxDist))
				{
					minCntOutlier = cntOutlier;
					minMaxDist = maxDist;
					inlierIdx1 = idx1;
					inlierIdx2 = idx2;
					minOutliers.clear();
					minOutliers.assign(outliers.begin(), outliers.end());
				}
			}
		}
		
		if (minCntOutlier > 0)
		{
			printf("in: ");
			for (int i = 0; i < inlierCandidates.size(); i++)
				printf("%d ", inlierCandidates[i]);
			printf("\nout: ");
			for (int i = 0; i < minOutliers.size(); i++)
				printf("%d ", minOutliers[i]);
			if (inlierCandidates.size() - minCntOutlier < 4)
				return;

			// erase outliers
			for (int i = 0; i < minOutliers.size(); i++)
			{
				int outlierIdx = minOutliers[i];
				inlierCandidates.erase(find(inlierCandidates.begin(), inlierCandidates.end(), outlierIdx));
			}
			printf("\nin: ");
			for (int i = 0; i < inlierCandidates.size(); i++)
				printf("%d ", inlierCandidates[i]);
			printf("\n");
		return;
		printf("outlier:%d dist:%.2lf i:%d j:%d\n", minCntOutlier, minMaxDist, inlierIdx1, inlierIdx2);
		printf("min outliers : ");
		for (int i = 0; i < minOutliers.size(); i++)
		{
		int outlierIdx = minOutliers[i];
		printf("%d ", outlierIdx);
		selectedIndices[outlierIdx] = -1;
		selectedTargets[outlierIdx] = Target();

		}
		getTracklet(solution, selectedIndices, selectedTargets, frames, frameNumber, useDummy);
		return;
		}



		// reconstruct dummy nodes
		if (useDummy)
		{
			vector<int> dummyIndices;

			for (int i = 0; i < n; i++)
			{
				if (selectedIndices[i] == -1)
					dummyIndices.push_back(i);
			}

			// TODO maximal 2 nodes
			// dummy maximal 1 nodes
			if (dummyIndices.size() > 1)
				return;
			if (dummyIndices.size() == 1)
			{
				int idx = dummyIndices[0];
				if (idx == 0)
				{
					Target target1 = selectedTargets[1], target2 = selectedTargets[2];
					Point p1 = target1.getCenterPoint(), p2 = target2.getCenterPoint(), p0 = (2 * p1 - p2);
					int width = target1.rect.width, height = target1.rect.height;
					Rect rect(p0.x - width / 2, p0.y - height / 2, width, height);
					Target target(rect, target1.hist);
					int targetSize = frames[frameNumber - n + idx].addTarget(target);
					selectedIndices[idx] = targetSize - 1;
					selectedTargets[idx] = target;
				}
				else if (idx == n - 1)
				{
					Target target1 = selectedTargets[n - 2], target2 = selectedTargets[n - 3];
					Point p1 = target1.getCenterPoint(), p2 = target2.getCenterPoint(), p0 = (2 * p1 - p2);
					int width = target1.rect.width, height = target1.rect.height;
					Rect rect(p0.x - width / 2, p0.y - height / 2, width, height);
					Target target(rect, target1.hist);
					int targetSize = frames[frameNumber - n + idx].addTarget(target);
					selectedIndices[idx] = targetSize - 1;
					selectedTargets[idx] = target;
				}
				else
				{
					Target target1 = selectedTargets[idx - 1], target2 = selectedTargets[idx + 1];
					Point p1 = target1.getCenterPoint(), p2 = target2.getCenterPoint(), p0 = (p1 + p2) / 2;
					int width = target1.rect.width, height = target1.rect.height;
					Rect rect(p0.x - width / 2, p0.y - height / 2, width, height);
					Target target(rect, target1.hist);
					int targetSize = frames[frameNumber - n + idx].addTarget(target);
					selectedIndices[idx] = targetSize - 1;
					selectedTargets[idx] = target;
				}
			}
			if (DEBUG)
			{
				printf("dummy check : ");
				for (int i = 0; i < n; i++)
					printf("%d ", selectedIndices[i]);
				cout << endl;
			}
		}

		
			

		// compute cost
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
	int cnt = 0;
	for (int i = 0; i < targets.size(); i++) 
	{
		Target& curTarget = targets[i];
		if (curTarget.found) 
		{
			continue;
		}
		
		// branch cutting using simple position comparison
		if (selectedTargets.size() > 0 && selectedIndices.back() != -1)
		{	
			Target& prevTarget = selectedTargets.back();
			Point motionErrVector = curTarget.getCenterPoint() - prevTarget.getCenterPoint();
			double motionCost = getNormValueFromVector(motionErrVector);
			if (motionCost > CONTINUOUS_MOTION_COST_THRE)
				continue;
		}
		selectedTargets.push_back(targets[i]);
		selectedIndices.push_back(i);
		getTracklet(solution, selectedIndices, selectedTargets, frames, frameNumber + 1, useDummy);
		selectedIndices.pop_back();
		selectedTargets.pop_back();
		cnt++;
	}
	
	// if not used, use dummy
	if ((n == 0 || cnt == 0) && useDummy)
	{
		selectedTargets.push_back(Target());
		selectedIndices.push_back(-1);
		getTracklet(solution, selectedIndices, selectedTargets, frames, frameNumber + 1, useDummy);
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

	int frameNum = 1, objectId;
	while (true)
	{
		printf("Frame #%d\n", frameNum);

		// set frame number
		cap.set(CV_CAP_PROP_POS_FRAMES, frameNum);

		// get frame
		Mat frame, frame_edge, frame_contour;
	
		cap >> frame;
		cvtColor(frame, frame_edge, COLOR_BGR2GRAY);
		cvtColor(frame, frame_contour, COLOR_BGR2GRAY);
		vector<Rect> & pedestrians = frames[frameNum].getPedestrians(), & prevPedestrians = frames[frameNum-1].getPedestrians();
		vector<Target> & curFrameTargets = frames[frameNum].getTargets(), prevFrameTargets = frames[frameNum - 1].getTargets();
		
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
			Mat cluster;
			cap >> cluster;
			segment.push_back(cluster);

			// draw found pedestrians
			vector<Rect>& pedestrians = frames[frameNum + i].getPedestrians();
			for (int j = 0; j < pedestrians.size(); j++)
			{
				Rect rect = pedestrians[j];
				rectangle(cluster, Rect(rect.x-10, rect.y-10, rect.width+20, rect.height+20), Scalar(255, 255, 255), 2);
				// rectangle(cluster, rect, Scalar(255, 255, 255), 2);
				
			}

			// reset 
			vector<Target>& targets = frames[frameNum + i].getTargets();
			for (int j = 0; j < targets.size(); j++)
			{
				targets[j].found = false;
				putText(cluster, std::to_string(j), targets[j].getCenterPoint(), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 3);
			}
		}

		bool useDummy = false;
		objectId = 0;

		while (true)
		{
			costMin = INFINITY;
			solution.clear();
			// printf("object %d : \n", objectId);
			getTracklet(solution, vector<int>(), vector<Target>(), frames, frameNum, useDummy);
			
			if (solution.size() < LOW_LEVEL_TRACKLETS)
			{
				if (useDummy)
					break;
				useDummy = true;
				cout << "Now, use dummy" << endl;
				continue;
			}
			cout << endl;

			if (DEBUG)
				printf("cost:%f\n", costMin);
			printf("obj:%d solution\n", objectId);
			for (int i = 0; i < solution.size(); i++)
			{
				Frame & curFrame = frames[frameNum + i];
				// draw center points
				Target & target = curFrame.getTarget(solution[i]);
				circle(frame, target.getCenterPoint(), 2, Scalar(255,255,255), 2);
				circle(frame, target.getCenterPoint(), 1, colors[objectId], 2);

				// draw found object in each frame
				rectangle(segment[i], curFrame.getPedestrian(solution[i]), colors[objectId], 4, 1);
				target.found = true;

				printf("%d ", solution[i]);
			}
			cout << endl;
			objectId++;
		}
		
		for (int i = 0; i < LOW_LEVEL_TRACKLETS; i++) 
		{
			resize(segment[i], segment[i], Size(400, 300));
			imshow("cluster-" + to_string(i+1), segment[i]);
		}
		
		imshow("result", frame);


		int key = waitKey(0);
		if (key == 27) break;
		if (key == (int)('m'))
		{
			frameNum = min(frameNum + LOW_LEVEL_TRACKLETS, MAX_FRAMES - LOW_LEVEL_TRACKLETS);
		}
		if (key == (int)('n'))
		{
			frameNum = min(frameNum + 1, MAX_FRAMES - 1);
		}
		else if (key == (int)('b'))
		{
			frameNum = max(frameNum - 1, 1);
		}
	}
}