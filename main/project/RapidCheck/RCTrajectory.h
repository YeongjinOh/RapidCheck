#ifndef RCTRAJECTORY_H
#define RCTRAJECTORY_H

#include "target.h"
typedef std::vector<Target> tracklet;

// Trajectory is already defined in cv
class RCTrajectory
{
private:
	int startSegmentNum, endSegmentNum;
	std::vector<Target> targets;
public:
	RCTrajectory(int segmentNum) : targets(0), startSegmentNum(segmentNum), endSegmentNum(segmentNum) { }
	RCTrajectory(std::vector<Target>& tr, int segmentNum) : targets(tr), startSegmentNum(segmentNum), endSegmentNum(segmentNum) { }
	void merge(tracklet tr);
	void mergeWithSegmentGap(tracklet tr, int diffNumSegment);
	void addTarget(Target target);
	int getStartSegmentNum();
	int getEndSegmentNum();
	Target getTarget(int idx);
	std::vector<Target> getTargets();
};

#endif