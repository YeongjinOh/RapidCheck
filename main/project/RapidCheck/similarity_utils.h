#include "tracking_utils.h"

/**
	calculate forward deviation error between two tracklets
*/
double calcForwardDeviationError(tracklet& trackletPrev, tracklet& trackletNext, int segmentIndexDiff);

/**
	calculate backward deviation error between two tracklets

*/
double calcBackwardDeviationError(tracklet& trackletPrev, tracklet& trackletNext, int segmentIndexDiff);

/**
	calculate motion similarity using deviation error between two tracklets
*/
double calcSimilarityMotion(tracklet& trackletPrev, tracklet& trackletNext, int segmentIndexDiff);

/**
	calculate appearance similarity between two tracklets
*/
double calcSimilarityAppearance(tracklet& trackletPrev, tracklet& trackletNext, int segmentIndexDiff);

/**
	calculate similarity between two tracklets
	it's linear sum of motion similarity and appearance similarity with varying ratio 
	@param segmentIndexDiff how many segments segmentNext is far away from segmentPrev(segmentIndexNext - segmentIndexPrev)
*/
double calcSimilarity(tracklet& trackletPrev, tracklet& trackletNext, int segmentIndexDiff);

/**
	check if two tracklets can be merged as valid car motion
*/
bool isValidCarMotion(tracklet& trackletPrev, tracklet& trackletNext);