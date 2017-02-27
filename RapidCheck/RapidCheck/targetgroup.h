#include "target.h"
#define MAX_LOST_FRAMES 15
class TargetGroup
{
public:
	std::vector<Target> targets;
	TargetGroup(){};

	TargetGroup(std::vector<Rect> rects);
	TargetGroup(std::vector<Rect2d> rects);
	void match(TargetGroup& currentFrameTargets);

	void addTargetToExistingTarget(Target &currentFrameTarget, int &index);
	void addNewTarget(Target &currentFrameTarget);
};