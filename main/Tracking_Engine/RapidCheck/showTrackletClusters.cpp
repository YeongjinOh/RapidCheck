#include "tracking_utils.h"
using namespace cv;
using namespace rc;

/**
	Show how to build tracklet from given detection results
*/
void showTrackletClusters()
{
	// set input video
	VideoCapture cap(filepath);

	// initialize colors	
	vector<Scalar> colors = getRandomColors();

	// build target detected frames
	vector<Frame> framePedestrians, frameCars;
	clock_t t = clock();
	if (SELECT_DETECTION_RESPONSE)
	{
		readTargets(cap, framePedestrians, frameCars);
	}
	else
	{
		detectTargets(cap, framePedestrians);
		// detectAndInsertResultIntoDB(cap);
	}
	t = clock() - t;
	if (DEBUG)
		printf("Detection takes %d(ms)\n", t);

	// build all tracklets
	t = clock();
	vector<Segment> segmentPedestrians, segmentCars;
	buildTracklets(framePedestrians, segmentPedestrians);
	buildTracklets(frameCars, segmentCars);

	// open windows
	for (int i = 0; i < LOW_LEVEL_TRACKLETS; i++)
	{
		namedWindow("Frame #" + to_string(i + 1));
		moveWindow("Frame #" + to_string(i + 1), 400*(i/3), (280*(i%3)));
	}

	Mat frame, frameSkipped;
	int numOfSegments = (numOfFrames - 1) / LOW_LEVEL_TRACKLETS;
	cap.set(CV_CAP_PROP_POS_FRAMES, startFrameNum);
	for (int segmentNumber = 0; segmentNumber < numOfSegments; segmentNumber++)
	{
		int frameNum = frameStep * (LOW_LEVEL_TRACKLETS * segmentNumber) + startFrameNum;
		cap.set(CV_CAP_PROP_POS_FRAMES, frameNum);
		vector<tracklet> &pedestrianTracklets = segmentPedestrians[segmentNumber].getTracklets(), &carTracklets = segmentCars[segmentNumber].getTracklets();
		for (int frameIdx = 0; frameIdx < LOW_LEVEL_TRACKLETS; frameIdx++)
		{
			frameNum = frameStep * (LOW_LEVEL_TRACKLETS * segmentNumber + frameIdx) + startFrameNum;
			
			cap >> frame;
			for (int i = 1; i < frameStep; i++)
				cap >> frameSkipped;
			
			// Detection responses
			vector<Rect> pedestrianRects = frameCars[LOW_LEVEL_TRACKLETS * segmentNumber + frameIdx].getRects();
			for (int i = 0; i < pedestrianRects.size(); i++) {
				Rect rect = pedestrianRects[i];
				rectangle(frame, Rect(rect.x - 10, rect.y - 10, rect.width + 20, rect.height + 20), WHITE, 2);
			}
			vector<Rect> carRects = frameCars[LOW_LEVEL_TRACKLETS * segmentNumber + frameIdx].getRects();
			for (int i = 0; i < carRects.size(); i++) {
				Rect rect = carRects[i];
				rectangle(frame, Rect(rect.x - 10, rect.y - 10, rect.width + 20, rect.height + 20), WHITE, 2);
			}

			// Tracklet results
			for (int objectId = 0; objectId < pedestrianTracklets.size(); objectId++)
			{
				Rect rect = pedestrianTracklets[objectId][frameIdx].getTargetArea();
				rectangle(frame, rect, colors[objectId], 4, 1);
				putText(frame, std::to_string(objectId), pedestrianTracklets[objectId][frameIdx].getCenterPoint() - Point(10, 10 + rect.height / 2), CV_FONT_HERSHEY_SIMPLEX, 1, colors[objectId], 3);
			}
			for (int objectId = 0; objectId < carTracklets.size(); objectId++)
			{
				int carId = objectId + pedestrianTracklets.size();
				Rect rect = carTracklets[objectId][frameIdx].getTargetArea();
				rectangle(frame, rect, colors[carId], 4, 1);
				putText(frame, std::to_string(carId), carTracklets[objectId][frameIdx].getCenterPoint() - Point(10, 10 + rect.height / 2), CV_FONT_HERSHEY_SIMPLEX, 1, colors[carId], 3);
			}

			resize(frame, frame, Size(400, 300));
			imshow("Frame #" + to_string(frameIdx + 1), frame);
		}
		int key = waitKey(0);
		if (key == 27) break;
		if (key == (int)('b'))
			segmentNumber -= 2;
		else if (key == (int)('r'))
			segmentNumber = -1;
	}
}