#include "target.h"
#define MAX_LOST_FRAMES 15
class TargetGroup
{
public:
	std::vector<Target> targets;
	TargetGroup(){};

	TargetGroup(std::vector<Rect> rects, std::vector<MatND> hists);
	TargetGroup(std::vector<Rect2d> rects, std::vector<MatND> hists);
	void match(TargetGroup& currentFrameTargets);

	void addTargetToExistingTarget(Target &currentFrameTarget, int &index);
	void addNewTarget(Target &currentFrameTarget);
};