#include "tracking_utils.h"
#include <time.h>

#define TRAJECTORY_MATCH_THRES 500

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
	cout << "Detection finished " << t << endl;

	// build all tracklets
	vector<Segment> segments;
	t = clock();
	buildTracklets(frames, segments);
	t = clock() - t;
	cout << "Tracklets built " << t << endl;

	int segmentIdxPrev = 0, segmentIdxNext = 1, targetIdxPrev = 0;
	Mat framePrev, frameNext;
	while (true)
	{
		// set frame
		int prevFrameNum = LOW_LEVEL_TRACKLETS * segmentIdxPrev + (LOW_LEVEL_TRACKLETS/2)  + START_FRAME_NUM;
		int nextFrameNum = LOW_LEVEL_TRACKLETS * segmentIdxNext + (LOW_LEVEL_TRACKLETS / 2) + START_FRAME_NUM;
		cap.set(CV_CAP_PROP_POS_FRAMES, prevFrameNum);
		cap >> framePrev;
		cap.set(CV_CAP_PROP_POS_FRAMES, nextFrameNum);
		cap >> frameNext;

		// show tracklet
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
		int objId = 0;
		for (int i = 0; i < trackletsPrev.size(); i++)
		{
			tracklet &trackletPrev = trackletsPrev[i];
			for (int j = 0; j < trackletPrev.size(); j++) 
			{
				circle(framePrev, trackletPrev[j].getCenterPoint(), 2, WHITE, 2);
				circle(framePrev, trackletPrev[j].getCenterPoint(), 1, colors[objId], 2);
			}
			if (i == targetIdxPrev)
				rectangle(framePrev, trackletPrev[LOW_LEVEL_TRACKLETS/2].rect, colors[objId], 2);
			objId++;
		}
		for (int i = 0; i < trackletsNext.size(); i++) 
		{
			tracklet &trackletNext = trackletsNext[i];
			for (int j = 0; j < trackletNext.size(); j++) 
			{
				circle(frameNext, trackletNext[j].getCenterPoint(), 2, WHITE, 2);
				circle(frameNext, trackletNext[j].getCenterPoint(), 1, colors[objId], 2);
			}
			objId++;
		}

		// put frame number

		rectangle(framePrev, Rect(0, 0, 150, 30), WHITE, -1);
		rectangle(frameNext, Rect(0, 0, 150, 30), WHITE, -1);
		putText(framePrev, "Frame #" + to_string(prevFrameNum), Point(10, 20), CV_FONT_HERSHEY_SIMPLEX, 0.7, BLACK, 2);
		putText(frameNext, "Frame #" + to_string(nextFrameNum), Point(10, 20), CV_FONT_HERSHEY_SIMPLEX, 0.7, BLACK, 2);
		imshow("prev", framePrev);
		imshow("next", frameNext);

		int key = waitKey(0);
		switch ((char)key) {
			case 'i': // prev Up
				if (segmentIdxPrev < segmentIdxNext)
					segmentIdxPrev++;	
				break;
			case 'o': // next Up
				if (segmentIdxNext + 1 < segments.size())
					segmentIdxNext++;
				break;
			case 'k': // prev Down
				if (segmentIdxPrev > 0)
					segmentIdxPrev--;
				break;
			case 'l': // next Down
				if (segmentIdxPrev < segmentIdxNext)
					segmentIdxNext--;
				break;
			case 't': // move target
				targetIdxPrev = (targetIdxPrev + 1) % trackletsPrev.size();
				break;
		}
	}

}