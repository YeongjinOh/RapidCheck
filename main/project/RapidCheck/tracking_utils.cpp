#include "tracking_utils.h"
using namespace cv;

// Calculate Internal Divison from a to b with m:n
int calcInternalDivision(int a, int b, int m, int n)
{
	return (a*n + b*m) / (m + n);
}

// Calculate Internal Divison from a to b with m:n
double calcInternalDivision(double a, double b, int m, int n)
{
	return (a*n + b*m) / (m + n);
}
// Generate random colors
vector<Scalar> getRandomColors()
{
	// random number generator
	RNG rng(0xFFFFFFFF);

	// initialize colors	
	vector<Scalar> colors;
	for (int i = 0; i < NUM_OF_COLORS; i++)
	{
		int icolor = (unsigned)rng;
		int minimumColor = 0;
		colors.push_back(Scalar(minimumColor + (icolor & 127), minimumColor + ((icolor >> 8) & 127), minimumColor + ((icolor >> 16) & 127)));
	}
	return colors;
}
// Calculate 2-d norm value of given vector
double getNormValueFromVector(Point p) {
	return sqrt(p.x*p.x + p.y*p.y);
}

// Reconstruct one dummy from right to left
void reconstructLeftDummy(vector<int>& selectedIndices, vector<Target>& selectedTargets, vector<Frame>& frames, int frameNumber, int idx)
{
	assert(idx >= 0 && idx + 2 < selectedTargets.size());
	Target target1 = selectedTargets[idx + 1], target2 = selectedTargets[idx + 2];
	Point p1 = target1.getCenterPoint(), p2 = target2.getCenterPoint(), p0 = (2 * p1 - p2);
	int width = target1.rect.width, height = target1.rect.height;
	Rect rect(p0.x - width / 2, p0.y - height / 2, width, height);
	Target target(rect, target1.hist);
	int targetSize = frames[frameNumber - LOW_LEVEL_TRACKLETS + idx].addTarget(target);
	selectedIndices[idx] = targetSize - 1;
	selectedTargets[idx] = target;
}

// Reconstruct one dummy from left to right
void reconstructRightDummy(vector<int>& selectedIndices, vector<Target>& selectedTargets, vector<Frame>& frames, int frameNumber, int idx)
{
	assert(idx >= 2 && idx < selectedTargets.size());
	Target target1 = selectedTargets[idx - 1], target2 = selectedTargets[idx - 2];
	Point p1 = target1.getCenterPoint(), p2 = target2.getCenterPoint(), p0 = (2 * p1 - p2);
	int width = target1.rect.width, height = target1.rect.height;
	Rect rect(p0.x - width / 2, p0.y - height / 2, width, height);
	Target target(rect, target1.hist);
	int targetSize = frames[frameNumber - LOW_LEVEL_TRACKLETS + idx].addTarget(target);
	selectedIndices[idx] = targetSize - 1;
	selectedTargets[idx] = target;
}

// Reconstruct one dummy in the middle of non-dummy nodes
void reconstructMiddleOneDummy(vector<int>& selectedIndices, vector<Target>& selectedTargets, vector<Frame>& frames, int frameNumber, int idx)
{
	assert(idx > 0 && idx + 1 < selectedTargets.size());
	Target target1 = selectedTargets[idx - 1], target2 = selectedTargets[idx + 1];
	Point p1 = target1.getCenterPoint(), p2 = target2.getCenterPoint(), p0 = (p1 + p2) / 2;
	int width = target1.rect.width, height = target1.rect.height;
	Rect rect(p0.x - width / 2, p0.y - height / 2, width, height);
	Target target(rect, target1.hist);
	int targetSize = frames[frameNumber - LOW_LEVEL_TRACKLETS + idx].addTarget(target);
	selectedIndices[idx] = targetSize - 1;
	selectedTargets[idx] = target;
}

// Reconstruct two consecutive dummy nodes in the middle of non-dummy nodes
void reconstructMiddleTwoDummies(vector<int>& selectedIndices, vector<Target>& selectedTargets, vector<Frame>& frames, int frameNumber, int idx1, int idx2)
{
	assert(idx1 > 0 && idx2 < selectedTargets.size() && idx1 + 1 == idx2);
	Target target1 = selectedTargets[idx1 - 1], target2 = selectedTargets[idx2 + 1];
	Point p1 = target1.getCenterPoint(), p2 = target2.getCenterPoint(), p_l = (2 * p1 + p2) / 3, p_r = (p1 + 2 * p2) / 3;
	int width_l = target1.rect.width, height_l = target1.rect.height, width_r = target2.rect.width, height_r = target2.rect.height;
	Rect rect_l(p_l.x - width_l / 2, p_l.y - height_l / 2, width_l, height_l), rect_r(p_r.x - width_r / 2, p_r.y - height_r / 2, width_r, height_r);
	Target target_l(rect_l, target1.hist), target_r(rect_r, target2.hist);
	int targetSize = frames[frameNumber - LOW_LEVEL_TRACKLETS + idx1].addTarget(target_l);
	selectedIndices[idx1] = targetSize - 1;
	selectedTargets[idx1] = target_l;
	targetSize = frames[frameNumber - LOW_LEVEL_TRACKLETS + idx2].addTarget(target_r);
	selectedIndices[idx2] = targetSize - 1;
	selectedTargets[idx2] = target_r;
}

// Print indices
void printIndices(vector<int>& selectedIndices)
{
	printf("indices : ");
	for (int i = 0; i < selectedIndices.size(); i++)
		printf("%d ", selectedIndices[i]);
	printf("\n");
}

// Build one optimal tracklet of given segment
void getTracklet(vector<int>& solution, vector<int>& selectedIndices, vector<Target>& selectedTargets, vector<Frame>& frames, int frameNumber, double& costMin, bool useDummy)
{

	// (n+1)-th frame
	int n = selectedTargets.size();

	// base case
	if (n == LOW_LEVEL_TRACKLETS) {

		// handle outliers
		vector<int> inlierCandidates;
		for (int i = 0; i < n; i++)
			if (selectedIndices[i] != -1)
				inlierCandidates.push_back(i);

		// at least 4 inliers needed
		if (inlierCandidates.size() < 4)
			return;

		// find the best pair of points which have
		// 1. minimum number of outlier
		// 2. minimum of maxDist
		double outlierDistThres = 50.0;
		int minCntOutlier = LOW_LEVEL_TRACKLETS;
		double minMaxDist = INFINITY;
		int inlierIdx1 = -1, inlierIdx2 = -1;
		
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
			// we can recover at most 2 outliers including dummy nodes
			if (inlierCandidates.size() - minCntOutlier < 4)
				return;

			// erase outliers
			for (int i = 0; i < minOutliers.size(); i++)
			{
				int outlierIdx = minOutliers[i];
				inlierCandidates.erase(find(inlierCandidates.begin(), inlierCandidates.end(), outlierIdx));
			}

			// change outliers with dummy nodes
			for (int i = 0; i < minOutliers.size(); i++)
			{
				int outlierIdx = minOutliers[i];
				selectedIndices[outlierIdx] = -1;
				selectedTargets[outlierIdx] = Target();
				useDummy = true;
			}
			getTracklet(solution, selectedIndices, selectedTargets, frames, frameNumber, costMin, useDummy);
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

			// reconstruct at most 2 dummy nodes
			if (dummyIndices.size() > 2)
				return;

			// case 1. only one dummy node
			if (dummyIndices.size() == 1)
			{
				int idx = dummyIndices[0];
				if (idx == 0)
					reconstructLeftDummy(selectedIndices, selectedTargets, frames, frameNumber, idx);
				else if (idx == n - 1)
					reconstructRightDummy(selectedIndices, selectedTargets, frames, frameNumber, idx);
				else
					reconstructMiddleOneDummy(selectedIndices, selectedTargets, frames, frameNumber, idx);
			}

			// case 2. two dummy nodes
			else if (dummyIndices.size() == 2)
			{
				int idx1 = dummyIndices[0], idx2 = dummyIndices[1];
				if (idx1 + 1 == idx2)
				{
					if (idx1 == 0 && idx2 == 1)
					{
						reconstructLeftDummy(selectedIndices, selectedTargets, frames, frameNumber, idx2);
						reconstructLeftDummy(selectedIndices, selectedTargets, frames, frameNumber, idx1);
					}
					else if (idx1 == n - 2 && idx2 == n - 1)
					{
						reconstructRightDummy(selectedIndices, selectedTargets, frames, frameNumber, idx1);
						reconstructRightDummy(selectedIndices, selectedTargets, frames, frameNumber, idx2);
					}
					else
						reconstructMiddleTwoDummies(selectedIndices, selectedTargets, frames, frameNumber, idx1, idx2);
				}
				else
				{
					if (idx1 == 0)
					{
						if (idx2 == n - 1)
							reconstructRightDummy(selectedIndices, selectedTargets, frames, frameNumber, idx2);
						else
							reconstructMiddleOneDummy(selectedIndices, selectedTargets, frames, frameNumber, idx2);
						reconstructLeftDummy(selectedIndices, selectedTargets, frames, frameNumber, idx1);
					}
					else {
						if (idx2 == n - 1)
							reconstructRightDummy(selectedIndices, selectedTargets, frames, frameNumber, idx2);
						else
							reconstructMiddleOneDummy(selectedIndices, selectedTargets, frames, frameNumber, idx2);
						reconstructMiddleOneDummy(selectedIndices, selectedTargets, frames, frameNumber, idx1);
					}
				}
			}

		}

		// compute cost
		// cost value is linear combination of cost_appearance and cost_motion
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

		// if this cost is minimum
		if (costMin > cost)
		{
			costMin = cost;
			// change solution
			solution.assign(selectedIndices.begin(), selectedIndices.end());
		}
		return;
	}

	// recursive process
	// assumption 1 : selectedIndices and selectedTargets have the same length
	// assumption 2 : the length of selectedTargets(= n) is less than or equal to LOW_LEVEL_TRACKLETS (= 6)
	// assumption 3 : frameNumber indicates (start frame number + offset) where offset is length of selectedTargets
	Frame& frame = frames[frameNumber];
	vector<Target>& targets = frame.getTargets();
	double cost;
	int cnt = 0;
	for (int i = 0; i < targets.size(); i++)
	{
		Target& curTarget = targets[i];
		if (curTarget.found)
			continue;

		// branch cutting using simple position comparison
		if (selectedTargets.size() > 0 && selectedIndices.back() != -1)
		{
			Target& prevTarget = selectedTargets.back();
			Point motionErrVector = curTarget.getCenterPoint() - prevTarget.getCenterPoint();
			double motionCost = getNormValueFromVector(motionErrVector);
			if (motionCost > CONTINUOUS_MOTION_COST_THRE)
				continue;
		}
		// recursive backtracking
		selectedTargets.push_back(targets[i]);
		selectedIndices.push_back(i);
		getTracklet(solution, selectedIndices, selectedTargets, frames, frameNumber + 1, costMin, useDummy);
		selectedIndices.pop_back();
		selectedTargets.pop_back();
		cnt++;
	}

	// if not used, use dummy
	if (useDummy && (n < 2 || cnt == 0))
	{
		// recursive backtracking
		selectedTargets.push_back(Target());
		selectedIndices.push_back(-1);
		getTracklet(solution, selectedIndices, selectedTargets, frames, frameNumber + 1, costMin, useDummy);
		selectedIndices.pop_back();
		selectedTargets.pop_back();
	}
}

// Detect targets in MAX_FRAMES frames
void detectTargets(App& app, VideoCapture& cap, vector<Frame>& frames)
{
	// initialize variables for histogram
	// Using 50 bins for hue and 60 for saturation
	int h_bins = 18; int s_bins = 6;
	int histSize[] = { h_bins, s_bins };
	// hue varies from 0 to 179, saturation from 0 to 255
	float h_ranges[] = { 0, 180 };
	float s_ranges[] = { 0, 256 };
	const float* ranges[] = { h_ranges, s_ranges };
	// Use the o-th and 1-st channels
	int channels[] = { 0, 1 };

	Mat frame;
	
	int frameNum = START_FRAME_NUM;
	totalFrameCount = cap.get(CV_CAP_PROP_FRAME_COUNT);
	cout << "total frame count : " << totalFrameCount << endl;
	for (int frameCnt = 0; frameCnt < MAX_FRAMES; frameCnt++, frameNum += FRAME_STEP) 
	{
		if (frameNum >= totalFrameCount)
		{
			printf("frameNum(%d) is bigger than total frame count(%d).\n", frameNum, totalFrameCount);
			return;
		}
		cap.set(CV_CAP_PROP_POS_FRAMES, frameNum);

		// get frame from the video
		cap >> frame;

		// stop the program if no more images
		if (frame.rows == 0 || frame.cols == 0)
			break;

		vector<Rect> found, found_filtered;

		// implement hog detection
		app.getHogResults(frame, found);
		

		size_t i, j;
		for (i = 0; i<found.size(); i++)
		{
			Rect& r = found[i];
			//for (j = 0; j<found.size(); j++)
		   	//	if (j != i && (r & found[j]) == r)
			//		break;
			//if (j == found.size())
				found_filtered.push_back(r);
		}
		for (i = 0; i<found_filtered.size(); i++)
		{
			Rect& r = found_filtered[i];
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
			//Rect r = found_filtered[i];
			Rect &r = found_filtered[i];
			r.x += r.width / 5;
			r.width = r.width * 3 / 5;
			r.y += r.height / 10;
			r.height = r.height * 4 / 5;

			cvtColor(frame(r), temp, COLOR_BGR2HSV);
			calcHist(&temp, 1, channels, Mat(), hist, 2, histSize, ranges, true, false);
			normalize(hist, hist, 0, 1, NORM_MINMAX, -1, Mat());
			hists.push_back(hist);
		}

		frames.push_back(Frame(frameNum, found_filtered, hists));
	}
}

// Read targets in MAX_FRAMES frames from DataBase
void readTargets(VideoCapture& cap, vector<Frame>& frames)
{
	// initialize variables for histogram
	// Using 50 bins for hue and 60 for saturation
	int h_bins = 18; int s_bins = 6;
	int histSize[] = { h_bins, s_bins };
	// hue varies from 0 to 179, saturation from 0 to 255
	float h_ranges[] = { 0, 180 };
	float s_ranges[] = { 0, 256 };
	const float* ranges[] = { h_ranges, s_ranges };
	// Use the o-th and 1-st channels
	int channels[] = { 0, 1 };

	Mat frame;

	int frameNum = START_FRAME_NUM;
	totalFrameCount = cap.get(CV_CAP_PROP_FRAME_COUNT);
	cout << "total frame count : " << totalFrameCount << endl;

	// read result from database and build mapFrameNumToPedestrians
	vector<vector<int > > res;
	map<int, vector<Rect> > mapFrameNumToPedestrians;
	db.selectDetection(res, videoId, START_FRAME_NUM, START_FRAME_NUM + FRAME_STEP * MAX_FRAMES, FRAME_STEP);
	for (int i = 0; i < res.size(); i++)
	{
		int frameNum = res[i][2], x = res[i][3], y = res[i][4], width = res[i][5], height = res[i][6];
		mapFrameNumToPedestrians[frameNum].push_back(Rect(x, y, width, height));
	}

	for (int frameCnt = 0; frameCnt < MAX_FRAMES; frameCnt++, frameNum += FRAME_STEP)
	{
		if (frameNum >= totalFrameCount)
		{
			printf("frameNum(%d) is bigger than total frame count(%d).\n", frameNum, totalFrameCount);
			return;
		}
		cap.set(CV_CAP_PROP_POS_FRAMES, frameNum);

		// get frame from the video
		cap >> frame;
		
		// create histograms
		vector<Rect>& found = mapFrameNumToPedestrians[frameNum];
		vector<MatND> hists;

		// shrink rect smaller
		double widthRatio = 0.5, heightRatio = 0.6, shiftUpperRatio = 0.0;
		for (int i = 0; i < found.size(); ++i)
		{
			Mat temp;
			MatND hist;
			//Rect r = found_filtered[i];
			Rect& r = found[i];
			r.x += r.width * (1-widthRatio) / 2;
			r.width = r.width * widthRatio;
			r.y += r.height * (1-heightRatio) / 2 - r.height * shiftUpperRatio;
			r.height = r.height * heightRatio;

			cvtColor(frame(r), temp, COLOR_BGR2HSV);
			calcHist(&temp, 1, channels, Mat(), hist, 2, histSize, ranges, true, false);
			normalize(hist, hist, 0, 1, NORM_MINMAX, -1, Mat());
			hists.push_back(hist);
		}

		frames.push_back(Frame(frameNum, found, hists));
	}
}

// Detect targets in MAX_FRAMES frames and insert result into DB
void detectAndInsertResultIntoDB(App& app, VideoCapture& cap)
{
	
	Mat frame;
	int frameNum = START_FRAME_NUM;
	totalFrameCount = cap.get(CV_CAP_PROP_FRAME_COUNT);
	cout << "total frame count : " << totalFrameCount << endl;
	int classId = 0;
	time_t t = clock();
	for (int frameCnt = 0; frameCnt < MAX_FRAMES; frameCnt++, frameNum += FRAME_STEP)
	{
		if (frameNum >= totalFrameCount)
		{
			printf("frameNum(%d) is bigger than total frame count(%d).\n", frameNum, totalFrameCount);
			return;
		}
		cap.set(CV_CAP_PROP_POS_FRAMES, frameNum);

		// get frame from the video
		cap >> frame;

		// stop the program if no more images
		if (frame.rows == 0 || frame.cols == 0)
			break;

		vector<Rect> found;
		// implement hog detection
		app.getHogResults(frame, found);
		for (int i = 0; i<found.size(); i++)
		{
			Rect& r = found[i];
			db.insertDetection(videoId, frameNum, classId, r.x, r.y, r.width, r.height);
		}
	}
	t = clock() - t;
	printf("detection and insertion into DB takes %d(ms) from %d to %d by %d-step", t, START_FRAME_NUM, START_FRAME_NUM + MAX_FRAMES, FRAME_STEP);
}

// Build all tracklets of given frames
void buildTracklets(vector<Frame>& frames, vector<Segment>& segments)
{
	// build all segments
	int frameNum = 0;
	for (int segmentNumber = 0; segmentNumber < NUM_OF_SEGMENTS && frameNum + LOW_LEVEL_TRACKLETS <= frames.size(); segmentNumber++, frameNum += LOW_LEVEL_TRACKLETS)
	{
		Segment segment(frameNum + START_FRAME_NUM);

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
}

// Build one optimal trajectory of given mid-level segment
void getTrajectory(vector<int>& solution) {
	// TODO
}

// Build all trajectories
void buildAllTrajectories(vector<Segment>& segments, vector<MidLevelSegemet>& mlSegments)
{

	for (int segmentNumber = 0; segmentNumber + MID_LEVEL_TRACKLETS <= segments.size(); segmentNumber += MID_LEVEL_TRACKLETS)
	{
		// create tracklet
		vector<int> solution;
		MidLevelSegemet mlSegment;

		// do not use dummy until no more solution built to optimize
		bool useDummy = false;
		while (true)
		{
			double costMin = INFINITY;
			solution.clear();

			// build solution
			getTrajectory(solution);

			// if no more solution
			if (solution.size() < LOW_LEVEL_TRACKLETS)
			{
				if (useDummy)
					break;
				useDummy = true;
				continue;
			}

			// for each solution
			RPTrajectory trajectory(segmentNumber);
			for (int i = 0; i < solution.size(); i++)
			{
				Segment& curSegment = segments[segmentNumber + i];
				tracklet& tracklet = curSegment.getTracklet(solution[i]);
				trajectory.merge(tracklet);
				// TODO : set found trues
			}
			mlSegment.addTrajectory(trajectory);
		}
		// TODO : not-found segments
		mlSegments.push_back(mlSegment);
	}
}