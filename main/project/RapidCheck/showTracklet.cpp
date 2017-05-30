#include "tracking_utils.h"
using namespace cv;
/**
	Build tracklets of all segements and then, show trace of tracklets

	@param app frame reader with basic parameters set
*/
void showTracklet(App app)
{
	// set input video
	VideoCapture cap(VIDEOFILE);

	// initialize colors	
	vector<Scalar> colors = getRandomColors();
	
	// build target detected frames
	vector<Frame> frames;
	//detectTargets(app, cap, frames);
	readTargets(cap, frames);
	cout << "Detection finished" << endl;

	// build all tracklets
	vector<Segment> segments;
	buildTracklets(frames, segments);
	cout << "Tracklets built" << endl;

	// show tracklets
	Mat frame;
	vector<Point> centers;
	vector<int> objectIds;
	vector<tracklet> entirePedestrianTracklets;
	int objectId = 0;
	for (int segmentNumber = 0; segmentNumber < NUM_OF_SEGMENTS; segmentNumber++)
	{
		Segment & segment = segments[segmentNumber];
		for (int frameIdx = 0; frameIdx < LOW_LEVEL_TRACKLETS; frameIdx++)
		{
			int frameNum = FRAME_STEP * (LOW_LEVEL_TRACKLETS * segmentNumber + frameIdx) + START_FRAME_NUM;
			cap.set(CV_CAP_PROP_POS_FRAMES, frameNum);
			cap >> frame;

			vector<tracklet>& pedestrianTracklets = segment.tracklets;
			for (int pedestrianNum = 0; pedestrianNum < pedestrianTracklets.size(); pedestrianNum++)
			{
				tracklet& pedestrianTracklet = pedestrianTracklets[pedestrianNum];
				Target& currentFramePedestrian = pedestrianTracklet[frameIdx];
				rectangle(frame, currentFramePedestrian.getTargetArea(), colors[(objectId + pedestrianNum) % NUM_OF_COLORS], 2);
				// circle(frame, currentFramePedestrian.getCenterPoint(), 2, RED, 2);
				centers.push_back(currentFramePedestrian.getCenterPoint());
				objectIds.push_back(objectId + pedestrianNum);
			}
			for (int i = 0; i < centers.size() && i < objectIds.size(); i++)
			{
				circle(frame, centers[i], 1, colors[objectIds[i]%NUM_OF_COLORS], 2);
			}
			imshow("tracklets", frame);

			// key handling
			int key = waitKey(10);
			if (key == 27) break;
			else if (key == (int)('r'))
			{
				centers.clear();
				objectIds.clear();
				segmentNumber = 0;
				break;
			}
			else if (key == (int)('c'))
			{
				centers.clear();
				objectIds.clear();
				break;
			}
			if (frameIdx == LOW_LEVEL_TRACKLETS - 1)
				objectId += pedestrianTracklets.size();
		}
	}
}