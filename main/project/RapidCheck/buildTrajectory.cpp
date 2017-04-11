#include "tracking_utils.h"

#define TRAJECTORY_MATCH_THRES 500

/**
	Build trajectories of all segements and then, show trace of tracklets

	@param app frame reader with basic parameters set
*/
void buildTrajectory(App app)
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
	detectTargets(app, cap, frames);
	cout << "Detection finished" << endl;

	// build all tracklets
	vector<Segment> segments;
	buildTracklets(frames, segments);
	cout << "Tracklets built" << endl;

	vector<RPTrajectory> trajectoriesFinished, trajectoriesStillBeingTracked;

	// build trajectories
	for (int segmentNum = 0; segmentNum < segments.size(); segmentNum++)
	{
		printf("segnum:%d\n", segmentNum);
		printf("size Finished:%d still:%d\n", trajectoriesFinished.size(), trajectoriesStillBeingTracked.size());

		Segment& segment = segments[segmentNum];
		vector<tracklet>& tracklets = segment.tracklets;

		// for each trajectory still being tracked
		for (vector<RPTrajectory>::iterator itTrajectories = trajectoriesStillBeingTracked.begin(); itTrajectories != trajectoriesStillBeingTracked.end(); itTrajectories++)
		{
			RPTrajectory& trajectory = *itTrajectories;
			int diffSegmentNum = segmentNum - trajectory.endSegmentNum;
			// if trajectory is finished
			if (diffSegmentNum > 2)
			{
				// trajectoriesFinished.push_back(trajectory);
				// trajectoriesStillBeingTracked.erase(itTrajectories);
				continue;
			}
			
			
			tracklet& curTrajectory = trajectory.targets;
			Point pl1 = curTrajectory[curTrajectory.size() - 2].getCenterPoint(), pl2 = curTrajectory[curTrajectory.size() - 1].getCenterPoint();
			double minCost = INFINITY;
			vector<tracklet>::iterator minTrackletIt;
			if (diffSegmentNum == 1)
			{
				// explore each tracklet in this segment
				for (vector<tracklet>::iterator itTracklets = tracklets.begin(); itTracklets != tracklets.end(); itTracklets++)
				{
					tracklet& tr = *itTracklets;
					double costForward, costBackward, curCost;
					Point pr1 = tr[0].getCenterPoint(), pr2 = tr[1].getCenterPoint();
					costForward = getNormValueFromVector(pr1 + pl1 - 2 * pl2);
					costBackward = getNormValueFromVector(pl2 + pr2 - 2 * pr1);
					curCost = costForward*costForward + costBackward*costBackward;
					if (minCost > curCost)
					{
						minCost = curCost;
						minTrackletIt = itTracklets;
					}
				}

				printf("minCost : %.2lf\n", minCost);
				if (minCost < TRAJECTORY_MATCH_THRES)
				{
					// merge
					trajectory.merge(*minTrackletIt);
					tracklets.erase(minTrackletIt);
					continue;
				}
			}
			else if (diffSegmentNum == 2)
			{
				for (vector<tracklet>::iterator itTracklets = tracklets.begin(); itTracklets != tracklets.end(); itTracklets++)
				{
					tracklet& tr = *itTracklets;
					
					Point pr1 = tr[0].getCenterPoint(), pr2 = tr[1].getCenterPoint();
					double curCost = getNormValueFromVector((9*pl2 - 7*pl1) - (9*pr1 - 7*pr2));
					if (minCost > curCost)
					{
						minCost = curCost;
						minTrackletIt = itTracklets;
					}
				}
			}
			

		}

		// push unselected tracklets
		for (int trackletNum = 0; trackletNum < tracklets.size(); trackletNum++)
		{
			trajectoriesStillBeingTracked.push_back(RPTrajectory(tracklets[trackletNum], segmentNum));
		}

		
	}


	printf("size Finished:%d still:%d\n", trajectoriesFinished.size(), trajectoriesStillBeingTracked.size());
	cout << "Built Trajectories" << endl;


	// show trajectories
	Mat frame, frameOrigin;
	int objectId = 0;
	for (int segmentNumber = 0; segmentNumber < NUM_OF_SEGMENTS; segmentNumber++)
	{
		Segment & segment = segments[segmentNumber];
		for (int frameIdx = 1; frameIdx <= LOW_LEVEL_TRACKLETS; frameIdx++)
		{
			int frameNum = LOW_LEVEL_TRACKLETS * segmentNumber + frameIdx + START_FRAME_NUM;
			cap.set(CV_CAP_PROP_POS_FRAMES, frameNum);
			cap >> frame;

			frame.copyTo(frameOrigin);

			// vector<tracklet>& pedestrianTracklets = segment.tracklets;
			for (int objectId = 0; objectId < trajectoriesStillBeingTracked.size(); objectId++)
			{
				RPTrajectory& trajectory = trajectoriesStillBeingTracked[objectId];
				if (segmentNumber < trajectory.startSegmentNum || segmentNumber > trajectory.endSegmentNum) continue;

				Target& currentFramePedestrian = trajectory.targets[6 * (segmentNumber - trajectory.startSegmentNum) + frameIdx - 1];
				rectangle(frame, currentFramePedestrian.rect, colors[(objectId) % NUM_OF_COLORS], 2);
				putText(frame, to_string(objectId), currentFramePedestrian.getCenterPoint(), 1, 1, WHITE, 1);
				// circle(frame, currentFramePedestrian.getCenterPoint(), 2, RED, 2);
			}
			vector<Rect> pedestrians = frames[LOW_LEVEL_TRACKLETS * segmentNumber + frameIdx].getPedestrians();
			for (int i = 0; i < pedestrians.size(); i++) {
				rectangle(frameOrigin, pedestrians[i], WHITE, 2);
			}
			
			imshow("tracklets", frame);
			imshow("origin", frameOrigin);

			// key handling
			int key = waitKey(0);
			if (key == 27) break;
			else if (key == (int)('r'))
			{
				segmentNumber = 0;
				break;
			}
			
		}
	}
	
}