#include "tracking_utils.h"

using namespace cv;

void showTrajectory ()
{
	VideoCapture cap(VIDEOFILE);

	// initialize colors	
	vector<Scalar> colors = getRandomColors();

	// show trajectories
	Mat frame;

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
			cap.set(CV_CAP_PROP_POS_FRAMES, frameNum);
			cap >> frame;

			// vector<tracklet>& pedestrianTracklets = segment.tracklets;
			for (int objectId = 0; objectId < trajectories.size(); objectId++)
			{
				RCTrajectory& trajectory = trajectories[objectId];
				if (segmentNumber < trajectory.getStartSegmentNum() || segmentNumber > trajectory.getEndSegmentNum()) continue;

				Target& currentFramePedestrian = trajectory.getTarget(LOW_LEVEL_TRACKLETS * (segmentNumber - trajectory.getStartSegmentNum()) + frameIdx);
				rectangle(frame, currentFramePedestrian.rect, colors[(objectId) % NUM_OF_COLORS], 2);
				putText(frame, to_string(objectId), currentFramePedestrian.getCenterPoint() - Point(10, 10 + currentFramePedestrian.rect.height / 2), 1, 1, colors[(objectId) % NUM_OF_COLORS], 1);
			}

			imshow("Trajectory", frame);

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
