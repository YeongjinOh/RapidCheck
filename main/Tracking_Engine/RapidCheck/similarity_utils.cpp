#include "similarity_utils.h"
// calculate forward deviation error between two tracklets
double calcForwardDeviationError(tracklet& trackletPrev, tracklet& trackletNext, int segmentIndexDiff)
{
	double forwardDeviationError = 0.0;
	// calculate forwardDeviation
	for (int i = 0; i < LOW_LEVEL_TRACKLETS / 2; i++)
	{
		int targetIdxPrev = LOW_LEVEL_TRACKLETS - 1 - i;
		Point& centerPointPrev = trackletPrev[targetIdxPrev].getCenterPoint();
		Point motionVectorPrev = centerPointPrev - trackletPrev[targetIdxPrev - 1].getCenterPoint();
		for (int j = 0; j < LOW_LEVEL_TRACKLETS / 2; j++)
		{
			int targetIdxNext = LOW_LEVEL_TRACKLETS*segmentIndexDiff + j;
			Point& centerPointNext = trackletNext[j].getCenterPoint();

			double err = getNormValueFromVector(centerPointPrev + (targetIdxNext - targetIdxPrev)*motionVectorPrev - centerPointNext);
			forwardDeviationError += err;
		}
	}
	return forwardDeviationError;
}

// calculate backward deviation error between two tracklets
double calcBackwardDeviationError(tracklet& trackletPrev, tracklet& trackletNext, int segmentIndexDiff)
{
	double backwardDeviationError = 0.0;
	// calculate forwardDeviation
	for (int i = 0; i < LOW_LEVEL_TRACKLETS / 2; i++)
	{
		int targetIdxNext = LOW_LEVEL_TRACKLETS*segmentIndexDiff + i;
		Point& centerPointNext = trackletNext[i].getCenterPoint();
		Point motionBackwardVectorNext = centerPointNext - trackletNext[i + 1].getCenterPoint();
		for (int j = 0; j < LOW_LEVEL_TRACKLETS / 2; j++)
		{
			int targetIdxPrev = LOW_LEVEL_TRACKLETS - 1 - j;
			Point& centerPointPrev = trackletPrev[targetIdxPrev].getCenterPoint();
			backwardDeviationError += getNormValueFromVector(centerPointNext + motionBackwardVectorNext * (targetIdxNext - targetIdxPrev) - centerPointPrev);
		}
	}
	return backwardDeviationError;
}

// calculate motion similarity using deviation error between two tracklets
double calcSimilarityMotion(tracklet& trackletPrev, tracklet& trackletNext, int segmentIndexDiff)
{
	double deviationError = calcForwardDeviationError(trackletPrev, trackletNext, segmentIndexDiff) + calcBackwardDeviationError(trackletPrev, trackletNext, segmentIndexDiff);
	Rect lastRectOfPrev = trackletPrev.back().getTargetArea(), firstRectOfNext = trackletNext.front().getTargetArea();
	int intersectionArea = (lastRectOfPrev & firstRectOfNext).area();
	double similarityJaccard = (double)intersectionArea / (lastRectOfPrev.area() + firstRectOfNext.area() - intersectionArea);
	double similarityMotion = exp(-deviationError / STANDARD_DEVIATION);
	if (segmentIndexDiff == 1)
		similarityMotion = similarityMotion * similarityJaccard;
	return  similarityMotion;
}

// calculate appearance similarity between two tracklets
double calcSimilarityAppearance(tracklet& trackletPrev, tracklet& trackletNext, int segmentIndexDiff)
{
	// Co-relation comparison
	int compareHistMethod = 0;
	MatND histSumPrev = trackletPrev[trackletPrev.size()-LOW_LEVEL_TRACKLETS].hist;;
	for (int j = trackletPrev.size() - LOW_LEVEL_TRACKLETS + 1; j < trackletPrev.size(); j++)
		histSumPrev += trackletPrev[j].hist;
	normalize(histSumPrev, histSumPrev, 0, 1, cv::NORM_MINMAX, -1, Mat());
	MatND histSumNext = trackletNext[0].hist;;
	for (int j = 1; j < LOW_LEVEL_TRACKLETS; j++)
		histSumNext += trackletNext[j].hist;
	normalize(histSumNext, histSumNext, 0, 1, cv::NORM_MINMAX, -1, Mat());
	double similarityAppearance = compareHist(histSumPrev, histSumNext, compareHistMethod);
	return similarityAppearance;
}

// calculate similarity between two tracklets
double calcSimilarity(tracklet& trackletPrev, tracklet& trackletNext, int segmentIndexDiff)
{
	double similarityAppearance = calcSimilarityAppearance(trackletPrev, trackletNext, segmentIndexDiff);
	double similarityMotion = calcSimilarityMotion(trackletPrev, trackletNext, segmentIndexDiff);
	double similarity = calcInternalDivision(similarityAppearance, similarityMotion, 1, segmentIndexDiff);
	return similarity;
}

double innerProduct(Point2d p1, Point2d p2)
{
	return p1.x * p2.x + p1.y + p2.y;
}

bool isValidMotion(tracklet& trackletPrev, tracklet& trackletNext)
{
	Rect lastRectOfPrev = trackletPrev.back().getTargetArea(), firstRectOfNext = trackletNext.front().getTargetArea();
	int intersectionArea = (lastRectOfPrev & firstRectOfNext).area();
	if (DEBUG)
		printf("intersection area : %d\n", intersectionArea);
	return intersectionArea > 0;
}

bool isValidCarMotion(tracklet& trackletPrev, tracklet& trackletNext)
{
	int n = trackletPrev.size();
	Point2d prevCarDirection = trackletPrev[n - 1].getCenterPoint() - trackletPrev[n-LOW_LEVEL_TRACKLETS].getCenterPoint();
	Point2d centerPrev = (trackletPrev[n - LOW_LEVEL_TRACKLETS/2 - 1].getCenterPoint() + trackletPrev[n - LOW_LEVEL_TRACKLETS / 2].getCenterPoint())/2;
	Point2d centerNext = (trackletNext[LOW_LEVEL_TRACKLETS / 2 - 1].getCenterPoint() + trackletNext[LOW_LEVEL_TRACKLETS / 2].getCenterPoint()) / 2;
	Point2d centerDirection = centerNext - centerPrev;
	return innerProduct(prevCarDirection, centerDirection) > 0;		
}