#ifndef TRACKING_UTILS_H
#define TRACKING_UTILS_H

#include "main.h"

#define MIXTURE_CONSTANT 0.1
#define LOW_LEVEL_TRACKLETS 6
#define MID_LEVEL_TRACKLETS 6
#define CONTINUOUS_MOTION_COST_THRE 30


#define MAX_FRAMES 31
#define NUM_OF_SEGMENTS (MAX_FRAMES - 1)/LOW_LEVEL_TRACKLETS
#define NUM_OF_MID_LEVEL_SEGMENTS NUM_OF_SEGMENTS/MID_LEVEL_TRACKLETS
#define NUM_OF_COLORS 64
#define DEBUG false
#define START_FRAME_NUM 0 // start frame number
#define FRAME_STEP 1


typedef vector<Target> tracklet;

struct Segment {
	vector<tracklet> tracklets;
	int startFrameNumber;

	Segment(int frameNum) : startFrameNumber(frameNum) { }
	void addTracklet(tracklet tr)
	{
		tracklets.push_back(tr);
	}
	tracklet getTracklet(int idx)
	{
		if (idx<tracklets.size())
			return tracklets[idx];
		return tracklet();
	}
};


// Trajectory is already defined in cv
struct RPTrajectory
{
	int startSegmentNum, endSegmentNum;
	vector<Target> targets;
	RPTrajectory(int segmentNum) : targets(0), startSegmentNum(segmentNum), endSegmentNum(segmentNum) { }
	RPTrajectory(vector<Target>& tr, int segmentNum) : targets(tr), startSegmentNum(segmentNum), endSegmentNum(segmentNum) { }
	void merge(tracklet tr)
	{
		targets.insert(targets.end(), tr.begin(), tr.end());
		endSegmentNum++;
	}
};

// Mid-level segments which consist of LOW_LEVEL_TRACKLETS*MID_LEVEL_TRACKLETS(36) frames
struct MidLevelSegemet
{
	vector<RPTrajectory> trajectories;
	void addTrajectory(RPTrajectory trajectory)
	{
		trajectories.push_back(trajectory);
	}
};

/**
	Calculate 2-d norm value of given vector

	@param p the 2-d vector from origin
	@return 2-d norm of given vector
*/
double getNormValueFromVector(Point p);

/**
	Reconstruct one dummy from right to left

	@param selectedIndices indices of selected target of each frames
	@param selectedTargets selected targets of each frames
	@param frame frames to reconstruct dummy
	@param frameNumber last frame number of this segment + 1
	@param idx index of dummy node(target) in selectedTargets which will be reconstructed
*/
void reconstructLeftDummy(vector<int>& selectedIndices, vector<Target>& selectedTargets, vector<Frame>& frames, int frameNumber, int idx);

/**
	Reconstruct one dummy from left to right

	@param selectedIndices indices of selected target of each frames
	@param selectedTargets selected targets of each frames
	@param frame frames to reconstruct dummy
	@param frameNumber last frame number of this segment + 1
	@param idx index of dummy node(target) in selectedTargets which will be reconstructed
*/
void reconstructRightDummy(vector<int>& selectedIndices, vector<Target>& selectedTargets, vector<Frame>& frames, int frameNumber, int idx);

/**
	Reconstruct one dummy in the middle of non-dummy nodes

	@param selectedIndices indices of selected target of each frames
	@param selectedTargets selected targets of each frames
	@param frame frames to reconstruct dummy
	@param frameNumber last frame number of this segment + 1
	@param idx index of dummy node(target) in selectedTargets which will be reconstructed
*/
void reconstructMiddleOneDummy(vector<int>& selectedIndices, vector<Target>& selectedTargets, vector<Frame>& frames, int frameNumber, int idx);

/**
	Reconstruct two consecutive dummy nodes in the middle of non-dummy nodes

	@param selectedIndices indices of selected target of each frames
	@param selectedTargets selected targets of each frames
	@param frame frames to reconstruct dummy
	@param frameNumber last frame number of this segment + 1
	@param idx1 index of left dummy node in selectedTargets which will be reconstructed
	@param idx2 index of right dummy node in selectedTargets which will be reconstructed
*/
void reconstructMiddleTwoDummies(vector<int>& selectedIndices, vector<Target>& selectedTargets, vector<Frame>& frames, int frameNumber, int idx1, int idx2);

/**
	Print indices

	@param selectedIndices indices of selected target of each frames
*/
void printIndices(vector<int>& selectedIndices);

/**
	Build one optimal tracklet of given segment.
	Optimal tracklet means list of targets with length of LOW_LEVEL_TRACKLETS(= 6) with minimal cost among all possible combinations

	@param solution solution indices of each frames which indicates optimal tracklet
	@param selectedIndices indices of selected target of each frames
	@param selectedTargets selected targets of each frames
	@param frames list of frames including this segment
	@param frameNumber number of frame checked in this recursion
	@param costMin minimum of cost so far
	@param useDummy boolean flag. if true, add dummy nodes and reconrstruct them.
*/
void getTracklet(vector<int>& solution, vector<int>& selectedIndices, vector<Target>& selectedTargets, vector<Frame>& frames, int frameNumber, double& costMin, bool useDummy = false);

/**
	Detect targets in MAX_FRAMES frames

	@param app frame reader with basic parameters set
	@param cap video variable
	@param frames list of frames to be implemented detection
*/
void detectTargets(App& app, VideoCapture& cap, vector<Frame>& frames);

/**
	Build all tracklets of given frames

	@param frames list of frames to be implemented detection
	@param segments list of segments to be built
*/
void buildTracklets(vector<Frame>& frames, vector<Segment>& segments);

/** Build one optimal trajectory of given mid-level segment
	
	@param solution solution indices of each frames which indicates optimal tracklet
*/
void getTrajectory(vector<int>& solution);

/**
	Build all tracklets of given frames

	@param segments list of segments to be matched
	@param mlSegments list of mide-level segments to be built
*/
void buildTrajectories(vector<Segment>& segments, vector<MidLevelSegemet>& mlSegments);

#endif