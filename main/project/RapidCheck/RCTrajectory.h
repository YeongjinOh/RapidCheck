#ifndef RCTRAJECTORY_H
#define RCTRAJECTORY_H

#include "target.h"
#include "config.h"
typedef std::vector<Target> tracklet;

// Trajectory is already defined in cv
class RCTrajectory
{
private:
	int startSegmentNum, endSegmentNum;
	std::vector<Target> targets;
	std::vector<int> cntDirections;
	std::vector<float> colorRatios;
	void increaseDirectionCount(tracklet &tr);
	std::vector<float> getColorRatioFromTracklet(tracklet &tr);
public:
	RCTrajectory(int segmentNum);
	RCTrajectory(std::vector<Target> &tr, int segmentNum);
	void merge(tracklet &tr);
	void mergeWithSegmentGap(tracklet &tr, int diffNumSegment);
	void addTarget(Target &target);
	int getStartSegmentNum();
	int getEndSegmentNum();
	Target getTarget(int idx);
	std::vector<Target> getTargets();
	std::vector<int> getCntDirections();
	std::vector<float> getColorRatios();
	void normalizeColorRatios();
};

#endif