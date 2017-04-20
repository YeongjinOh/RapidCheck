#include "tracking_utils.h"
#include <time.h>
using namespace cv;
/**
	calculate forward deviation error between two tracklets

	@param segmentIndexDiff how many segments segmentNext is far away from segmentPrev (segmentIndexNext - segmentIndexPrev)
*/
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
			cout << centerPointPrev << endl << centerPointNext << endl << motionVectorPrev << endl << err << endl << endl;
			forwardDeviationError += err;
		}
	}
	return forwardDeviationError;
}

/**
	calculate backward deviation error between two tracklets

	@param segmentIndexDiff how many segments segmentNext is far away from segmentPrev (segmentIndexNext - segmentIndexPrev)
*/
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

double calcSimilarityMotion(tracklet& trackletPrev, tracklet& trackletNext, int segmentIndexDiff)
{
	double deviationError = calcForwardDeviationError(trackletPrev, trackletNext, segmentIndexDiff) + calcBackwardDeviationError(trackletPrev, trackletNext, segmentIndexDiff);
	return exp(-deviationError / STANDARD_DEVIATION);
}
/**
	calculate and compare similarity between two tracklets

	@param app frame reader with basic parameters set
*/
void compareSimilarity(App app)
{
	// set input video
	VideoCapture cap(VIDEOFILE);

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

	// build target detected frames
	vector<Frame> frames;
	clock_t t = clock();
	detectTargets(app, cap, frames);
	t = clock() - t;
	cout << "Detection finished " << t << " with size of " << frames.size() << " frames" << endl;

	// build all tracklets
	vector<Segment> segments;
	t = clock();
	buildTracklets(frames, segments);
	t = clock() - t;
	cout << "Tracklets built " << t << endl;

	int segmentIdxPrev = 0, segmentIdxNext = 1, targetIdxPrev = 0;
	Mat framePrev, frameNext;
	int compareHistMethods[4] = {CV_COMP_CORREL, CV_COMP_CHISQR, CV_COMP_INTERSECT, CV_COMP_BHATTACHARYYA };
	int compareHistMethod = compareHistMethods[0];
	bool switchM = true, switchS = true, switchF = false, switchB = false; // switch for similarity, forward, backward error
	while (true)
	{
		// set frame
		int prevFrameNum = FRAME_STEP*(LOW_LEVEL_TRACKLETS * segmentIdxPrev + (LOW_LEVEL_TRACKLETS / 2)) + START_FRAME_NUM;
		int nextFrameNum = FRAME_STEP*(LOW_LEVEL_TRACKLETS * segmentIdxNext + (LOW_LEVEL_TRACKLETS / 2)) + START_FRAME_NUM;
		cap.set(CV_CAP_PROP_POS_FRAMES, prevFrameNum);
		cap >> framePrev;
		cap.set(CV_CAP_PROP_POS_FRAMES, nextFrameNum);
		cap >> frameNext;

		// show tracklet
		if (segments.size() <= segmentIdxNext) break;
		Segment &segmentPrev = segments[segmentIdxPrev], &segmentNext = segments[segmentIdxNext];
		vector<tracklet> &trackletsPrev = segmentPrev.tracklets, &trackletsNext = segmentNext.tracklets;
		
		// check if targets exist
		if (trackletsPrev.size() == 0)
		{
			segmentIdxPrev++;
			continue;
		}
		if (trackletsNext.size() == 0)
		{
			segmentIdxNext++;
			continue;
		}

		// draw tracklets
		int objectId = 0;
		for (int i = 0; i < trackletsPrev.size(); i++)
		{
			tracklet &trackletPrev = trackletsPrev[i];
			for (int j = 0; j < trackletPrev.size(); j++) 
			{
				circle(framePrev, trackletPrev[j].getCenterPoint(), 2, WHITE, 2);
				circle(framePrev, trackletPrev[j].getCenterPoint(), 1, colors[objectId], 2);
			}
			objectId++;
		}
		for (int i = 0; i < trackletsNext.size(); i++) 
		{
			tracklet &trackletNext = trackletsNext[i];
			for (int j = 0; j < trackletNext.size(); j++) 
			{
				circle(frameNext, trackletNext[j].getCenterPoint(), 2, WHITE, 2);
				circle(frameNext, trackletNext[j].getCenterPoint(), 1, colors[objectId], 2);
			}
			objectId++;
		}
		tracklet& trackletPrev = trackletsPrev[targetIdxPrev];
		Target& targetPrev = trackletPrev[LOW_LEVEL_TRACKLETS / 2];
		rectangle(framePrev, targetPrev.rect, colors[targetIdxPrev], 2);
		MatND histSumPrev = trackletsPrev[targetIdxPrev][0].hist;
		for (int i = 1; i < trackletsPrev[targetIdxPrev].size(); i++)
			histSumPrev += trackletsPrev[targetIdxPrev][i].hist;
		normalize(histSumPrev, histSumPrev, 0, 1, NORM_MINMAX, -1, Mat());

		// calculate similarities
		for (int i = 0; i < trackletsNext.size(); i++)
		{
			MatND histSumNext = trackletsNext[i][0].hist;;
			for (int j = 1; j < trackletsNext[i].size(); j++)
				histSumNext += trackletsNext[i][j].hist;
			normalize(histSumNext, histSumNext, 0, 1, NORM_MINMAX, -1, Mat());
			double similarityAppearance = compareHist(histSumPrev, histSumNext, compareHistMethod);			
			Target & targetNext = trackletsNext[i][LOW_LEVEL_TRACKLETS / 2];
			if (switchS)
				putText(frameNext, to_string(similarityAppearance), targetNext.getCenterPoint(), CV_FONT_HERSHEY_SIMPLEX, 0.7, GREEN, 2);
			
			
			tracklet& trackletNext = trackletsNext[i];
			double similarityMotion = calcSimilarityMotion(trackletPrev, trackletNext, segmentIdxNext - segmentIdxPrev);
			if (switchM)
				putText(frameNext, to_string(similarityMotion), targetNext.getCenterPoint() + Point(0, 20), CV_FONT_HERSHEY_SIMPLEX, 0.7, RED, 2);
			double forwardDeviationError = calcForwardDeviationError(trackletPrev, trackletNext, segmentIdxNext - segmentIdxPrev);
			double backwardDeviationError = calcBackwardDeviationError(trackletPrev, trackletNext, segmentIdxNext - segmentIdxPrev);
			if (switchF)
				putText(frameNext, to_string(forwardDeviationError), targetNext.getCenterPoint() + Point(0, 40), CV_FONT_HERSHEY_SIMPLEX, 0.7, BLUE, 2);
			if (switchB)
				putText(frameNext, to_string(backwardDeviationError), targetNext.getCenterPoint() + Point(0, 60), CV_FONT_HERSHEY_SIMPLEX, 0.7, BLUE, 2);
			

		}

		// put frame number
		rectangle(framePrev, Rect(0, 0, 150, 30), WHITE, -1);
		rectangle(frameNext, Rect(0, 0, 150, 30), WHITE, -1);
		putText(framePrev, "Frame #" + to_string(prevFrameNum), Point(10, 20), CV_FONT_HERSHEY_SIMPLEX, 0.7, BLACK, 2);
		putText(frameNext, "Frame #" + to_string(nextFrameNum), Point(10, 20), CV_FONT_HERSHEY_SIMPLEX, 0.7, BLACK, 2);

		// show result
		imshow("prev", framePrev);
		imshow("next", frameNext);

		int key = waitKey(0);
		switch ((char)key) {
			case 'i': // prev Up
				if (segmentIdxPrev < segmentIdxNext)
					segmentIdxPrev++;
				while (segments[segmentIdxPrev].tracklets.size() == 0) { segmentIdxPrev++; }
				targetIdxPrev = 0;
				break;
			case 'o': // next Up
				if (segmentIdxNext + 1 < segments.size())
					segmentIdxNext++;
				while (segments[segmentIdxNext].tracklets.size() == 0) { segmentIdxNext++; }
				break;
			case 'k': // prev Down
				if (segmentIdxPrev > 0)
					segmentIdxPrev--;
				targetIdxPrev = 0;
				while (segments[segmentIdxPrev].tracklets.size() == 0) { segmentIdxPrev--; }
				break;
			case 'l': // next Down
				if (segmentIdxPrev < segmentIdxNext)
					segmentIdxNext--;
				while (segments[segmentIdxNext].tracklets.size() == 0) { segmentIdxNext--; }
				break;
			case 't': // move target
				targetIdxPrev = (targetIdxPrev + 1) % trackletsPrev.size();
				break;
			case 'c':
				compareHistMethod = (compareHistMethod + 1) % 4;
				break;
			case 's':
				switchS = !switchS;
				break;
			case 'm':
				switchM = !switchM;
				break;
			case 'f':
				switchF = !switchF;
				break;
			case 'b':
				switchB = !switchB;
				break;

		}
	}

}