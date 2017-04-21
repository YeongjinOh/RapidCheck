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
	return exp(-deviationError / STANDARD_DEVIATION);
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