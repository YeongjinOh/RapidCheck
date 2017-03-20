#include "tracking_utils.h"

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
	if ((n < 2 || cnt == 0) && useDummy)
	{
		// recursive backtracking
		selectedTargets.push_back(Target());
		selectedIndices.push_back(-1);
		getTracklet(solution, selectedIndices, selectedTargets, frames, frameNumber + 1, costMin, useDummy);
		selectedIndices.pop_back();
		selectedTargets.pop_back();
	}
}
