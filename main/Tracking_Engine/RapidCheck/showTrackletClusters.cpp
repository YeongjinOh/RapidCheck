#include "tracking_utils.h"
using namespace cv;
/**
	Show how to build tracklet from given detection results
*/
void showTrackletClusters()
{
	// set input video
	VideoCapture cap(VIDEOFILE);

	// random number generator
	RNG rng(0xFFFFF0FF);
	
	Mat frame, targetImage;
	Target target;
	bool hasTarget = false;

	// initialize colors	
	vector<Scalar> colors;
	for (int i = 0; i < NUM_OF_COLORS; i++) 
	{
		int icolor = (unsigned)rng;
		int minimumColor = 0;
		colors.push_back(Scalar(minimumColor + (icolor & 127), minimumColor + ((icolor >> 8) & 127), minimumColor + ((icolor >> 16) & 127)));
	}
		
	// build target detected frames
	vector<Frame> frames, framePedestrians, frameCars;
	if (SELECT_DETECTION_RESPONSE)
	{
		readTargets(cap, framePedestrians, frameCars);
	}
	else
	{
		detectTargets(cap, frames);
	}
	if (USE_PEDESTRIANS_ONLY)
	{
		frames = framePedestrians;
	}
	else
	{
		frames = frameCars;
	}
	if (DEBUG)
		printf("Detection finished\n");
	
	// open windows
	for (int i = 0; i < LOW_LEVEL_TRACKLETS; i++)
	{
		namedWindow("Frame #" + to_string(i + 1));
		moveWindow("Frame #" + to_string(i + 1), 400*(i/3), (280*(i%3)));
	}
	int frameNum = 0, objectId;
	while (true)
	{
		if (DEBUG)
			printf("\n\nFrame #%d", frameNum);

		// set frame number
		cap.set(CV_CAP_PROP_POS_FRAMES, FRAME_STEP * (frameNum-1) + START_FRAME_NUM + LOW_LEVEL_TRACKLETS - 1);

		// get frame
		Mat frame;
		cap >> frame;

		// draw detection responses of the first frame in this segment with green rectangle
		vector<Rect> & pedestrians = frames[frameNum + LOW_LEVEL_TRACKLETS - 1].getRects();
		// &prevPedestrians = frames[frameNum + LOW_LEVEL_TRACKLETS - 1 - 1].getRects();
		//for (int i = 0; i < pedestrians.size(); i++)
		//	rectangle(frame, pedestrians[i], GREEN, 2, 1);

		// create tracklet
		vector<int> solution;
		vector<Mat> segment; // set of k frames
		for (int i = 0; i < LOW_LEVEL_TRACKLETS; i++)
		{
			// set frame number
			cap.set(CV_CAP_PROP_POS_FRAMES, FRAME_STEP * (frameNum-1) + START_FRAME_NUM + i);
			// get frame
			Mat cluster;
			cap >> cluster;
			segment.push_back(cluster);

			// draw detection responses with white rectangle in each cluster
			vector<Rect>& pedestrians = frames[frameNum + i].getRects();
			for (int j = 0; j < pedestrians.size(); j++)
			{
				Rect rect = pedestrians[j];
				rectangle(cluster, Rect(rect.x-10, rect.y-10, rect.width+20, rect.height+20), WHITE, 2);
			}

			vector<Target>& targets = frames[frameNum + i].getTargets();
			for (int j = 0; j < targets.size(); j++)
			{
				// reset found flag
				targets[j].found = false;
				// putText(cluster, std::to_string(j), targets[j].getCenterPoint(), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 3);
			}
		}

		// do not use dummy until no more solution built to optimize
		bool useDummy = false;
		objectId = 0;
		while (true)
		{
			double costMin = INFINITY;
			solution.clear();
			
			// build solution
			// getTracklet(solution, vector<int>(), vector<Target>(), frames, frameNum, costMin, useDummy);
			getTrackletOfCars(solution, vector<int>(), vector<Target>(), frames, frameNum, costMin, useDummy);

			// if no more solution
			if (solution.size() < LOW_LEVEL_TRACKLETS)
			{
				if (useDummy)
					break;
				useDummy = true;
				if (DEBUG)
					printf("\nSolutions from dummy nodes reconstruction");
				continue;
			}
			if (DEBUG)
				printf("\n");

			if (DEBUG)
			{
				printf("cost:%f\n", costMin);
				printf("object %d : ", objectId);
			}

			// for each solution
			for (int i = 0; i < solution.size(); i++)
			{
				Frame & curFrame = frames[frameNum + i];
				// draw center points
				Target & target = curFrame.getTarget(solution[i]);
				circle(frame, target.getCenterPoint(), 2, Scalar(255,255,255), 2);
				circle(frame, target.getCenterPoint(), 1, colors[objectId], 2);

				// draw found object in each frame
				rectangle(segment[i], curFrame.getRect(solution[i]), colors[objectId], 4, 1);
				putText(segment[i], std::to_string(objectId), target.getCenterPoint() - Point(10,10+target.getTargetArea().height/2), CV_FONT_HERSHEY_SIMPLEX, 1, colors[objectId], 3);
				target.found = true;
				if (DEBUG)
					printf("%d ", solution[i]);
			}
			objectId++;
		}
		
		// show all clusters
		for (int i = 0; i < LOW_LEVEL_TRACKLETS; i++) 
		{
			resize(segment[i], segment[i], Size(400, 300));
			imshow("Frame #" + to_string(i+1), segment[i]);
		}

		// show result trace
		imshow("tracklet", frame);

		// key handling
		int key = waitKey(0);
		if (key == 27) break;
		if (key == (int)('m'))
			frameNum = min(frameNum + LOW_LEVEL_TRACKLETS, MAX_FRAMES - LOW_LEVEL_TRACKLETS);
		else if (key == (int)('v'))
			frameNum = max(frameNum - LOW_LEVEL_TRACKLETS, 1);
		else if (key == (int)('n'))
			frameNum = min(frameNum + 1, MAX_FRAMES - 1);
		else if (key == (int)('b'))
			frameNum = max(frameNum - 1, 1);
		else if (key == (int)('r'))
			frameNum = 1;
	}
}