#include "main.h"

#define MIXTURE_CONSTANT 0.1
#define LOW_LEVEL_TRACKLETS 6
#define CONTINUOUS_MOTION_COST_THRE 30

typedef vector<Target> tracklet;

struct Segment {
	vector<tracklet> tracklets;
	int startFrameNumber;

	Segment(int frameNum) : startFrameNumber(frameNum) { }
	void addTracklet(tracklet tr)
	{
		tracklets.push_back(tr);
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