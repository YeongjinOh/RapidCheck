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

void RCTrajectory::merge(tracklet tr)
{
	targets.insert(targets.end(), tr.begin(), tr.end());
	endSegmentNum++;
	increaseDirectionCount(tr);
}

void RCTrajectory::increaseDirectionCount(tracklet tr)
{
	Point p = tr.back().getCenterPoint() - tr[0].getCenterPoint();
	if (p.x == 0 && p.y == 0) return;
	int directionClass = motionVectorToDirectionClass(p);
	cntDirections[directionClass]++;
}

void RCTrajectory::mergeWithSegmentGap(tracklet tr, int diffNumSegment)
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
	targets.insert(targets.end(), tr.begin(), tr.end());
	increaseDirectionCount(tr);
	endSegmentNum += diffNumSegment;
}

void RCTrajectory::addTarget(Target target)
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