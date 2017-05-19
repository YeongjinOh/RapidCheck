#include "RCTrajectory.h"
#include "config.h"
#include <cmath>

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

int motionVectorToDirectionClass(cv::Point p)
{
	double unit = 2 * PI / NUM_OF_DIRECTIONS;
	double deg = atan2(p.y, p.x);
	for (int i = 1; i <= NUM_OF_DIRECTIONS; i++)
	{
		if (deg > PI - unit * i)
			return i - 1;
	}
	return -1;
}

RCTrajectory::RCTrajectory(int segmentNum) : targets(0), startSegmentNum(segmentNum), endSegmentNum(segmentNum), cntDirections(NUM_OF_DIRECTIONS, 0), colorRatios(NUM_OF_COLOR_CLASSES, 0) {}
RCTrajectory::RCTrajectory(std::vector<Target> &tr, int segmentNum) : targets(tr), startSegmentNum(segmentNum), endSegmentNum(segmentNum), cntDirections(NUM_OF_DIRECTIONS, 0), colorRatios(NUM_OF_COLOR_CLASSES, 0) {
	colorRatios = getColorRatioFromTracklet(tr);
}

std::vector<float> RCTrajectory::getColorRatioFromTracklet(tracklet &tr)
{
	std::vector<float> colorHistSum(NUM_OF_COLOR_CLASSES, 0);
	Target &representativeTargetInTracklet = tr[LOW_LEVEL_TRACKLETS / 2];
	MatND &hist = representativeTargetInTracklet.hist;
	float totalHistSum = 0.0;
	for (int i = 0; i < hist.rows; i++)
	{
		int currentColorIdx = i / NUM_HUE_GROUP_SIZE;
		for (int j = 0; j < hist.cols; j++)
		{
			float currentHistValue = hist.at<float>(i, j);
			colorHistSum[currentColorIdx] += currentHistValue;
			totalHistSum += currentHistValue;
		}
	}
	for (int i = 0; i < colorHistSum.size(); i++)
	{
		colorHistSum[i] /= totalHistSum;
	}
	return colorHistSum;
}

void RCTrajectory::merge(tracklet &tr)
{
	targets.insert(targets.end(), tr.begin(), tr.end());
	endSegmentNum++;
	increaseDirectionCount(tr);
	std::vector<float> colorRatio = getColorRatioFromTracklet(tr);
	for (int i = 0; i < colorRatios.size(); i++)
	{
		colorRatios[i] += colorRatio[i];
	}
}

void RCTrajectory::increaseDirectionCount(tracklet &tr)
{
	Point p = tr.back().getCenterPoint() - tr[0].getCenterPoint();
	if (p.x == 0 && p.y == 0) return;
	int directionClass = motionVectorToDirectionClass(p);
	cntDirections[directionClass]++;
}

void RCTrajectory::mergeWithSegmentGap(tracklet &tr, int diffNumSegment)
{
	Rect RectPrev = targets.back().rect, RectNext = tr[0].rect;
	int numberOfDummies = (diffNumSegment - 1) * LOW_LEVEL_TRACKLETS;
	for (int i = 1; i <= numberOfDummies; i++) {
		int predictedX = calcInternalDivision(RectPrev.x, RectNext.x, i, numberOfDummies + 1 - i),
			predictedY = calcInternalDivision(RectPrev.y, RectNext.y, i, numberOfDummies + 1 - i),
			predictedWidth = calcInternalDivision(RectPrev.width, RectNext.width, i, numberOfDummies + 1 - i),
			predictedHeight = calcInternalDivision(RectPrev.height, RectNext.height, i, numberOfDummies + 1 - i);
		Rect predictedRect(predictedX, predictedY, predictedWidth, predictedHeight);
		targets.push_back(Target(predictedRect));
	}
	endSegmentNum += diffNumSegment - 1;
	merge(tr);
}

void RCTrajectory::addTarget(Target &target)
{
	targets.push_back(target);
	int numOfTargets = targets.size();
	if (numOfTargets % 6 == 1)
	{
		endSegmentNum = startSegmentNum + (numOfTargets / 6);
	}
}

int RCTrajectory::getStartSegmentNum() 
{
	return startSegmentNum;
}

int RCTrajectory::getEndSegmentNum()
{
	return endSegmentNum;
}

Target RCTrajectory::getTarget(int idx)
{
	if (targets.size() > idx)
		return targets[idx];
	return Target();
}

std::vector<Target> RCTrajectory::getTargets()
{
	return targets;
}

std::vector<int> RCTrajectory::getCntDirections()
{
	return cntDirections;
}

std::vector<float> RCTrajectory::getColorRatios()
{
	return colorRatios;
}

void RCTrajectory::normalizeColorRatios()
{
	float sumColorRatios = 0.0;
	for (int i = 0; i < colorRatios.size(); i++)
	{
		sumColorRatios += colorRatios[i];
	}
	for (int i = 0; i < colorRatios.size(); i++)
	{
		colorRatios[i] /= sumColorRatios;
	}
}