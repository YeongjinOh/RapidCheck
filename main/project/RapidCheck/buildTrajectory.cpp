#include "tracking_utils.h"
#include "similarity_utils.h"
#include "RCTrajectory.h"

using namespace cv;

/**
	Show trajectory
*/
void showTrajectory(vector<Frame>& frames, vector<RCTrajectory>& trajectories)
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

				// vector<tracklet>& pedestrianTracklets = segment.tracklets;
				for (int objectId = 0; objectId < trajectories.size(); objectId++)
				{
					RCTrajectory& trajectory = trajectories[objectId];
					if (segmentNumber < trajectory.getStartSegmentNum() || segmentNumber > trajectory.getEndSegmentNum()) continue;

					Target& currentFramePedestrian = trajectory.getTarget(LOW_LEVEL_TRACKLETS * (segmentNumber - trajectory.getStartSegmentNum()) + frameIdx);

					// Rect rect = currentFramePedestrian.getTargetArea(), roi = Rect (rect.x+rect.width/4, rect.y+rect.height/4, rect.width/2, rect.height/2);
					// Scalar mean = cv::mean(frame(roi));
					// rectangle(frame, currentFramePedestrian.getTargetArea(), mean, 2);
					rectangle(frame, currentFramePedestrian.getTargetArea(), colors[(objectId) % NUM_OF_COLORS], 2);

					if (INSERT_TRACKING_INTO_DB)
					{
						db.insertTracking(videoId, objectId, frameNum, currentFramePedestrian.getTargetArea().x, currentFramePedestrian.getTargetArea().y, currentFramePedestrian.getTargetArea().width, currentFramePedestrian.getTargetArea().height);
					}
					putText(frame, to_string(objectId), currentFramePedestrian.getCenterPoint() - Point(10, 10 + currentFramePedestrian.getTargetArea().height / 2), 1, 1, colors[(objectId) % NUM_OF_COLORS], 1);
					// circle(frame, currentFramePedestrian.getCenterPoint(), 2, RED, 2);
				}
				vector<Rect> pedestrians = frames[LOW_LEVEL_TRACKLETS * segmentNumber + frameIdx].getPedestrians();
				for (int i = 0; i < pedestrians.size(); i++) {
					rectangle(frameOrigin, pedestrians[i], WHITE, 2);
				}

				rectangle(frame, Rect(0, 0, 180, 30), WHITE, -1);
				rectangle(frameOrigin, Rect(0, 0, 180, 30), WHITE, -1);
				putText(frame, "Frame #" + to_string(frameNum), Point(10, 20), CV_FONT_HERSHEY_SIMPLEX, 0.7, BLACK, 2);
				putText(frameOrigin, "Frame #" + to_string(frameNum), Point(10, 20), CV_FONT_HERSHEY_SIMPLEX, 0.7, BLACK, 2);
				imshow("Trajectory", frame);
				imshow("Detection response", frameOrigin);

				// key handling
				int key = waitKey(130);

				if (key == 27) break;
				else if (key == (int)('r'))
				{
					segmentNumber = 0;
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

/**
	Build trajectories of all segements and then, show trace of tracklets

	@param app frame reader with basic parameters set
*/
void buildTrajectory(App app)
{
	// set input video
	VideoCapture cap(VIDEOFILE);

	// build target detected frames
	vector<Frame> frames;
	clock_t t = clock();
	if (SELECT_DETECTION_RESPONSE)
	{
		readTargets(cap, frames);
	}
	else
	{
		detectTargets(app, cap, frames);
		//detectAndInsertResultIntoDB(app, cap);
	}
	t = clock() - t;
	printf("Detection takes %d(ms)\n", t);
	
	// build all tracklets
	vector<Segment> segments;
	t = clock();
	buildTracklets(frames, segments);
	t = clock() - t;
	printf("Tracking takes %d(ms)\n", t);

	// build Trajectory
	vector<RCTrajectory> trajectoriesFinished, trajectoriesStillBeingTracked;
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
		// for each trajectory still being tracked
		for (vector<RCTrajectory>::iterator itTrajectories = trajectoriesStillBeingTracked.begin(); itTrajectories != trajectoriesStillBeingTracked.end();)
		{
			RCTrajectory& RCTrajectory = *itTrajectories;
			int diffSegmentNum = segmentNum - RCTrajectory.getEndSegmentNum();
			// if trajectory is finished
			if (diffSegmentNum > MAXIMUM_LOST_SEGMENTS)
			{
				trajectoriesFinished.push_back(*itTrajectories);
				itTrajectories = trajectoriesStillBeingTracked.erase(itTrajectories);
				continue;
			}
			else
			{
				itTrajectories++;
			}
			
			tracklet& curTrajectory = RCTrajectory.getTargets();
			vector<tracklet>::iterator maxTrackletIt;

			double maxSimilarity = 0.0;
			for (vector<tracklet>::iterator itTracklets = curSegmentTracklets.begin(); itTracklets != curSegmentTracklets.end(); itTracklets++)
			{
				tracklet& tr = *itTracklets;
				if (!isValidCarMotion(curTrajectory, tr)) continue;
				double curSimilarity = calcSimilarity(curTrajectory, tr, diffSegmentNum);
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
					RCTrajectory.merge(*maxTrackletIt);
				}
				else
				{
					RCTrajectory.mergeWithSegmentGap(*maxTrackletIt, diffSegmentNum);
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
	}
	for (vector<RCTrajectory>::iterator itTrajectories = trajectoriesStillBeingTracked.begin(); itTrajectories != trajectoriesStillBeingTracked.end(); itTrajectories++)
	{
		trajectoriesFinished.push_back(*itTrajectories);
	}
	trajectoriesStillBeingTracked.clear();

	cout << "Built Trajectories" << endl;
	
	// insert direction counts into DB
	if (INSERT_OBJECT_INFO_INTO_DB)
	{
		insertObjectInfoIntoDB(trajectoriesFinished);
	}
		

	// show Trajectory
	showTrajectory(frames, trajectoriesFinished);
}