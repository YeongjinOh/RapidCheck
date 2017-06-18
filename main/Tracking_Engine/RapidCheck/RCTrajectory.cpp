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

RCTrajectory::RCTrajectory(int segmentNum) : targets(0), cntValidTracklets(0), startSegmentNum(segmentNum), endSegmentNum(segmentNum), cntDirections(NUM_OF_DIRECTIONS, 0), directionRatios(NUM_OF_DIRECTIONS), colorRatios(NUM_OF_COLOR_CLASSES, 0) {}
RCTrajectory::RCTrajectory(std::vector<Target> &tr, int segmentNum) : targets(tr), cntValidTracklets(1), startSegmentNum(segmentNum), endSegmentNum(segmentNum), cntDirections(NUM_OF_DIRECTIONS, 0), directionRatios(NUM_OF_DIRECTIONS), colorRatios(NUM_OF_COLOR_CLASSES, 0) {
	colorRatios = getColorRatioFromTracklet(tr);
	increaseDirectionCount(tr);
}

std::vector<float> RCTrajectory::getColorRatioFromTracklet(tracklet &tr)
{
	Target &representativeTargetInTracklet = tr[LOW_LEVEL_TRACKLETS / 2];
	MatND &hist = representativeTargetInTracklet.histColor;
	float totalHistSum = 0.0;
	for (int h = 0; h < NUM_OF_HUE_BINS_COLOR; h++)
	{
		for (int s = 0; s < NUM_OF_SAT_BINS_COLOR; s++)
		{
			for (int v = 0; v < NUM_OF_VAL_BINS_COLOR; v++)
			{
				totalHistSum += hist.at<float>(h, s, v);
			}
		}
	}
	float blackRatio = 0.0, whiteRatio = 0.0;
	std::vector<float> colorRatio(NUM_OF_HUE_BINS_COLOR + 2, 0.0);

	for (int h = 0; h < NUM_OF_HUE_BINS_COLOR; h++)
	{
		for (int s = 0; s < NUM_OF_SAT_BINS_COLOR; s++)
		{
			for (int v = 0; v < NUM_OF_VAL_BINS_COLOR; v++)
			{
				if (v == 0)
				{
					colorRatio[NUM_OF_HUE_BINS_COLOR + 1] += hist.at<float>(h, s, v) / totalHistSum;
				}
				else if (s == 0)
				{
					colorRatio[NUM_OF_HUE_BINS_COLOR] += hist.at<float>(h, s, v) / totalHistSum;
				}
				else
				{
					colorRatio[h] += hist.at<float>(h, s, v) / totalHistSum;
				}
			}
		}
	}
	return colorRatio;
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
	cntValidTracklets++;
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
	Rect RectPrev = targets.back().getTargetArea(), RectNext = tr[0].getTargetArea();
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

std::vector<float> RCTrajectory::getDirectionRatios()
{
	return directionRatios;
}

std::vector<float> RCTrajectory::getColorRatios()
{
	return colorRatios;
}

void RCTrajectory::normalizeColorRatios()
{
	for (int i = 0; i < cntDirections.size(); i++) {
		directionRatios[i] = (float)cntDirections[i] / cntValidTracklets;
	}
	for (int i = 0; i < colorRatios.size(); i++)
	{
		colorRatios[i] /= cntValidTracklets;
	}
}

double RCTrajectory::calcSpeed()
{
	Point posDiff = targets.front().getCenterPoint() - targets.back().getCenterPoint();
	return sqrt(posDiff.x * posDiff.x + posDiff.y * posDiff.y) / (targets.size() - 1);
}