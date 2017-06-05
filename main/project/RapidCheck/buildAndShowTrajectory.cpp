#include "tracking_utils.h"
#include "similarity_utils.h"
#include "RCTrajectory.h"

using namespace cv;

/**
	Show trajectory
*/
void showTrajectory(vector<Frame>& framePedestrians, vector<Frame>& frameCars, vector<RCTrajectory>& trajectoryPedestrians, vector<RCTrajectory>& trajectoryCars)
{
	VideoCapture cap(VIDEOFILE);

	// initialize colors	
	vector<Scalar> colors = getRandomColors();

	// show trajectories
	Mat frame, frameOrigin, frameSkipped;
	int timeToSleep = 130;
	while (true) {
		int objectId = 0;
		cap.set(CV_CAP_PROP_POS_FRAMES, START_FRAME_NUM);
		for (int segmentNumber = 0; segmentNumber < NUM_OF_SEGMENTS; segmentNumber++)
		{
			// Segment & segment = segments[segmentNumber];
			printf("segmentNum : %d\n", segmentNumber);
			for (int frameIdx = 0; frameIdx < LOW_LEVEL_TRACKLETS; frameIdx++)
			{
				int frameNum = FRAME_STEP * (LOW_LEVEL_TRACKLETS * segmentNumber + frameIdx) + START_FRAME_NUM;
				// cap.set(CV_CAP_PROP_POS_FRAMES, frameNum);
				cap >> frame;
				for (int i = 1; i < FRAME_STEP; i++)
					cap >> frame;

				frame.copyTo(frameOrigin);

				// draw pedestrian
				for (int objectId = 0; objectId < trajectoryPedestrians.size(); objectId++)
				{
					RCTrajectory& trajectory = trajectoryPedestrians[objectId];
					if (segmentNumber < trajectory.getStartSegmentNum() || segmentNumber > trajectory.getEndSegmentNum()) continue;
					Target& currentFramePedestrian = trajectory.getTarget(LOW_LEVEL_TRACKLETS * (segmentNumber - trajectory.getStartSegmentNum()) + frameIdx);
					rectangle(frame, currentFramePedestrian.getTargetArea(), colors[(objectId) % NUM_OF_COLORS], 2);
					if (INSERT_TRACKING_INTO_DB)
					{
						db.insertTracking(videoId, objectId, frameNum, currentFramePedestrian.getTargetArea().x, currentFramePedestrian.getTargetArea().y, currentFramePedestrian.getTargetArea().width, currentFramePedestrian.getTargetArea().height);
					}
					putText(frame, "Pede #" + to_string(objectId), currentFramePedestrian.getCenterPoint() - Point(30, 10 + currentFramePedestrian.getTargetArea().height / 2), 1, 1.5, colors[(objectId) % NUM_OF_COLORS], 2);
				}

				for (int objectId = 0; objectId < trajectoryCars.size(); objectId++)
				{
					RCTrajectory& trajectory = trajectoryCars[objectId];
					if (segmentNumber < trajectory.getStartSegmentNum() || segmentNumber > trajectory.getEndSegmentNum()) continue;
					Target& currentFramePedestrian = trajectory.getTarget(LOW_LEVEL_TRACKLETS * (segmentNumber - trajectory.getStartSegmentNum()) + frameIdx);
					Scalar carColor = colors[(objectId + trajectoryPedestrians.size()) % NUM_OF_COLORS];
					rectangle(frame, currentFramePedestrian.getTargetArea(), carColor, 2);
					if (INSERT_TRACKING_INTO_DB)
					{
						db.insertTracking(videoId, objectId, frameNum, currentFramePedestrian.getTargetArea().x, currentFramePedestrian.getTargetArea().y, currentFramePedestrian.getTargetArea().width, currentFramePedestrian.getTargetArea().height);
					}
					putText(frame, "Car #" + to_string(objectId), currentFramePedestrian.getCenterPoint() - Point(30, 10 + currentFramePedestrian.getTargetArea().height / 2), 1, 1.5, carColor, 2);
				}

				vector<Rect> pedestrianRects = framePedestrians[LOW_LEVEL_TRACKLETS * segmentNumber + frameIdx].getRects();
				for (int i = 0; i < pedestrianRects.size(); i++) {
					rectangle(frameOrigin, pedestrianRects[i], WHITE, 2);
				}
				vector<Rect> carRects = frameCars[LOW_LEVEL_TRACKLETS * segmentNumber + frameIdx].getRects();
				for (int i = 0; i < carRects.size(); i++) {
					rectangle(frameOrigin, carRects[i], CYAN, 2);
				}

				rectangle(frame, Rect(0, 0, 180, 30), WHITE, -1);
				rectangle(frameOrigin, Rect(0, 0, 180, 30), WHITE, -1);
				putText(frame, "Frame #" + to_string(frameNum), Point(10, 20), CV_FONT_HERSHEY_SIMPLEX, 0.7, BLACK, 2);
				putText(frameOrigin, "Frame #" + to_string(frameNum), Point(10, 20), CV_FONT_HERSHEY_SIMPLEX, 0.7, BLACK, 2);
				imshow("Trajectory", frame);
				imshow("Detection response", frameOrigin);

				// key handling
				int key = waitKey(timeToSleep);

				if (key == 27) break;
				else if (key == (int)('r'))
				{
					segmentNumber = NUM_OF_SEGMENTS;
					break;
				}
				else if (key == (int)('p'))
				{
					key = waitKey(0);
				}
				else if (key == (int)('['))
				{
					timeToSleep += 10;
				}
				else if (key == (int)(']'))
				{
					timeToSleep -= 10;
					if (timeToSleep < 1)
						timeToSleep = 1;
				}
				else if (key == (int)('b'))
				{
					segmentNumber = max(0, segmentNumber-10);
					frameNum = FRAME_STEP * (LOW_LEVEL_TRACKLETS * segmentNumber + frameIdx) + START_FRAME_NUM;
					cap.set(CV_CAP_PROP_POS_FRAMES, frameNum);
					break;
				}
			}
		}
		waitKey(0);
	}
}

void buildTrajectory(vector<Segment>& segments, vector<RCTrajectory>& trajectories)
{

	// build Trajectory
	vector<RCTrajectory> trajectoriesStillBeingTracked;
	bool useOnlineTracking = true;
	if (!useOnlineTracking)
	{
		for (int segmentNum = 0; segmentNum < segments.size(); segmentNum++)
		{
			vector<tracklet>& tracklets = segments[segmentNum].tracklets;
			for (int t = 0; t < tracklets.size(); t++)
				trajectoriesStillBeingTracked.push_back(RCTrajectory(tracklets[t], segmentNum));
		}
	}

	// build trajectories if use online tracking
	for (int segmentNum = 0; segmentNum < segments.size() && useOnlineTracking; segmentNum++)
	{
		Segment& segment = segments[segmentNum];
		vector<tracklet>& curSegmentTracklets = segment.getTracklets();
		printf("segmentNum : %d, # of tracklets : %d\n", segmentNum, curSegmentTracklets.size());

		// move trajectories finished from trajectoriesStillBeingTracked to trajectoriesFinished
		for (vector<RCTrajectory>::iterator itTrajectories = trajectoriesStillBeingTracked.begin(); itTrajectories != trajectoriesStillBeingTracked.end();)
		{
			RCTrajectory& curTrajectory = *itTrajectories;
			int diffSegmentNum = segmentNum - curTrajectory.getEndSegmentNum();
			// if trajectory is finished
			if (diffSegmentNum > MAXIMUM_LOST_SEGMENTS)
			{
				trajectories.push_back(*itTrajectories);
				itTrajectories = trajectoriesStillBeingTracked.erase(itTrajectories);
				continue;
			}
			else
			{
				itTrajectories++;
			}
		}

		// bipartite graph from i to j
		vector<vector<double> > graphSimilarity = vector<vector<double> >(trajectoriesStillBeingTracked.size(), vector<double>(curSegmentTracklets.size(), 0.0));
		for (int i = 0; i < trajectoriesStillBeingTracked.size(); i++)
		{
			for (int j = 0; j < curSegmentTracklets.size(); j++)
			{
				RCTrajectory& curTrajectory = trajectoriesStillBeingTracked[i];
				tracklet &curTracklet = curTrajectory.getTargets(), &nextTracklet = curSegmentTracklets[j];
				
				//if (isValidCarMotion(curTracklet, nextTracklet))
				if (isValidMotion(curTracklet, nextTracklet))
				{
					int diffSegmentNum = segmentNum - curTrajectory.getEndSegmentNum();
					double similarity = calcSimilarity(curTracklet, nextTracklet, diffSegmentNum);
					if (similarity > TRAJECTORY_MATCH_SIMILARITY_THRES)
						graphSimilarity[i][j] = similarity;
				}
				printf("%.2lf ", graphSimilarity[i][j]);
			}
			std::cout << endl;
		}
		std::cout << endl;
		vector<bool> mergedNextTracket(curSegmentTracklets.size(), false);
		while (true)
		{
			double maxSimilarity = 0.0;
			int curMaxIdx = -1, nextMaxIdx = -1;
			for (int curIdx = 0; curIdx < graphSimilarity.size(); curIdx++)
			{
				for (int nextIdx = 0; nextIdx < graphSimilarity[curIdx].size(); nextIdx++)
				{
					if (maxSimilarity < graphSimilarity[curIdx][nextIdx])
					{
						maxSimilarity = graphSimilarity[curIdx][nextIdx];
						curMaxIdx = curIdx;
						nextMaxIdx = nextIdx;
					}
				}
			}
			if (maxSimilarity < TRAJECTORY_MATCH_SIMILARITY_THRES)
				break;

			// merge
			RCTrajectory& curTrajectory = trajectoriesStillBeingTracked[curMaxIdx];
			tracklet &nextTracklet = curSegmentTracklets[nextMaxIdx];
			int diffSegmentNum = segmentNum - curTrajectory.getEndSegmentNum();
			if (diffSegmentNum == 1)
			{
				curTrajectory.merge(nextTracklet);
			}
			else
			{
				curTrajectory.mergeWithSegmentGap(nextTracklet, diffSegmentNum);
			}

			// remove similarities
			mergedNextTracket[nextMaxIdx] = true;
			for (int i = 0; i < graphSimilarity.size(); i++)
				graphSimilarity[i][nextMaxIdx] = 0.0;
			for (int j = 0; j < graphSimilarity[curMaxIdx].size(); j++)
				graphSimilarity[curMaxIdx][j] = 0.0;
		}

		for (int trackletNum = 0; trackletNum < curSegmentTracklets.size(); trackletNum++)
		{
			if (!mergedNextTracket[trackletNum])
			{
				trajectoriesStillBeingTracked.push_back(RCTrajectory(curSegmentTracklets[trackletNum], segmentNum));
			}
		}

		/*
		// for each trajectory still being tracked
		for (vector<RCTrajectory>::iterator itTrajectories = trajectoriesStillBeingTracked.begin(); itTrajectories != trajectoriesStillBeingTracked.end(); itTrajectories++)
		{
		RCTrajectory& curTrajectory = *itTrajectories;
		int diffSegmentNum = segmentNum - curTrajectory.getEndSegmentNum();
		tracklet& curTracklet = curTrajectory.getTargets();
		vector<tracklet>::iterator maxTrackletIt;

		double maxSimilarity = 0.0;
		for (vector<tracklet>::iterator itTracklets = curSegmentTracklets.begin(); itTracklets != curSegmentTracklets.end(); itTracklets++)
		{
		tracklet& nextTracklet = *itTracklets;
		if (!isValidCarMotion(curTracklet, nextTracklet)) continue;
		double curSimilarity = calcSimilarity(curTracklet, nextTracklet, diffSegmentNum);
		if (maxSimilarity < curSimilarity)
		{
		maxSimilarity = curSimilarity;
		maxTrackletIt = itTracklets;
		}
		}
		printf("maxSim : %.2lf\n", maxSimilarity);
		if (maxSimilarity >= TRAJECTORY_MATCH_SIMILARITY_THRES)
		{
		// merge
		if (diffSegmentNum == 1)
		{
		curTrajectory.merge(*maxTrackletIt);
		}
		else
		{
		curTrajectory.mergeWithSegmentGap(*maxTrackletIt, diffSegmentNum);
		}
		curSegmentTracklets.erase(maxTrackletIt);
		continue;
		}
		}


		// push unselected tracklets
		for (int trackletNum = 0; trackletNum < curSegmentTracklets.size(); trackletNum++)
		{
		trajectoriesStillBeingTracked.push_back(RCTrajectory(curSegmentTracklets[trackletNum], segmentNum));
		}
		*/
	}
	for (vector<RCTrajectory>::iterator itTrajectories = trajectoriesStillBeingTracked.begin(); itTrajectories != trajectoriesStillBeingTracked.end(); itTrajectories++)
	{
		trajectories.push_back(*itTrajectories);
	}
}

/**
	Build trajectories of all segements and then, show trace of tracklets

	@param app frame reader with basic parameters set
*/
void buildAndShowTrajectory(App app)
{
	// set input video
	VideoCapture cap(VIDEOFILE);

	// build target detected frames
	vector<Frame> framePedestrians, frameCars;
	clock_t t = clock();
	if (SELECT_DETECTION_RESPONSE)
	{
		readTargets(cap, framePedestrians, frameCars);
	}
	else
	{
		detectTargets(app, cap, framePedestrians);
		//detectAndInsertResultIntoDB(app, cap);
	}
	t = clock() - t;
	printf("Detection takes %d(ms)\n", t);
	
	// build all tracklets
	t = clock();
	vector<Segment> segmentPedestrians, segmentCars;
	buildTracklets(framePedestrians, segmentPedestrians);
	buildTracklets(frameCars, segmentCars);
	t = clock() - t;
	printf("Tracklet takes %d(ms)\n", t);


	// build trajectories
	t = clock();
	vector<RCTrajectory> trajectoryPedestrians, trajectoryCars;
	buildTrajectory(segmentPedestrians, trajectoryPedestrians);
	buildTrajectory(segmentCars, trajectoryCars);
	t = clock() - t;
	printf("Trajectory takes %d(ms)\n", t);
	

	// insert direction counts into DB
	if (INSERT_OBJECT_INFO_INTO_DB)
	{
		insertObjectInfoIntoDB(trajectoryPedestrians);
		insertObjectInfoIntoDB(trajectoryCars);
	}
		
	// show Trajectory
	
	showTrajectory(framePedestrians, frameCars, trajectoryPedestrians, trajectoryCars);
}