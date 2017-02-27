#include "target.h"

class TargetGroup
{
public:
	std::vector<Target> targets;
	TargetGroup(std::vector<Rect> rects);
	void match(TargetGroup& currentFrameTargets);
private:
	void addTargetToExistingTarget(Target &currentFrameTarget, int &index);
	void addNewTarget(Target &currentFrameTarget);
};