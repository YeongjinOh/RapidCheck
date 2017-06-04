#include "tracking_utils.h"

using namespace cv;

void showTrajectory ()
{
	VideoCapture cap(VIDEOFILE);

	// initialize colors	
	vector<Scalar> colors = getRandomColors();

	// show trajectories
	Mat frame, frameOrigin, frameSkipped;

	vector<Frame> frames, framePedestrians, frameCars;
	readTargets(cap, framePedestrians, frameCars);
	if (USE_PEDESTRIANS_ONLY)
	{
		frames = framePedestrians;
	}
	else
	{
		frames = frameCars;
	}
	
	cap.set(CV_CAP_PROP_POS_FRAMES, START_FRAME_NUM);

	// read trajectories from database
	vector<RCTrajectory> trajectories;
	readTrajectories(trajectories);
	int objectId = 0;
	for (int segmentNumber = 0; segmentNumber < NUM_OF_SEGMENTS; segmentNumber++)
	{
		// Segment & segment = segments[segmentNumber];
		for (int frameIdx = 0; frameIdx < LOW_LEVEL_TRACKLETS; frameIdx++)
		{
			int frameNum = FRAME_STEP * (LOW_LEVEL_TRACKLETS * segmentNumber + frameIdx) + START_FRAME_NUM;
			
			cap >> frame;
			frame.copyTo(frameOrigin);
			for (int i = 1; i < FRAME_STEP; i++)
			{
				cap >> frameSkipped;
			}

			// vector<tracklet>& pedestrianTracklets = segment.tracklets;
			for (int objectId = 0; objectId < trajectories.size(); objectId++)
			{
				RCTrajectory& trajectory = trajectories[objectId];
				if (segmentNumber < trajectory.getStartSegmentNum() || segmentNumber > trajectory.getEndSegmentNum()) continue;
				Target& currentFramePedestrian = trajectory.getTarget(LOW_LEVEL_TRACKLETS * (segmentNumber - trajectory.getStartSegmentNum()) + frameIdx);
				rectangle(frame, currentFramePedestrian.getTargetArea(), colors[(objectId) % NUM_OF_COLORS], 2);
				putText(frame, to_string(objectId), currentFramePedestrian.getCenterPoint() - Point(10, 10 + currentFramePedestrian.getTargetArea().height / 2), 1, 1, colors[(objectId) % NUM_OF_COLORS], 1);
			}
			vector<Rect> pedestrians = frames[LOW_LEVEL_TRACKLETS * segmentNumber + frameIdx].getRects();
			for (int i = 0; i < pedestrians.size(); i++) 
			{
				rectangle(frameOrigin, pedestrians[i], WHITE, 2);
			}

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

		}
	}
}
