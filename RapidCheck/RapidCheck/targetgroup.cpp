#include "targetgroup.h"
TargetGroup::TargetGroup(std::vector<Rect> rects)
{
	for (int i = 0; i < rects.size(); ++i)
	{
		targets.push_back(Target(rects[i]));
	}
}
TargetGroup::TargetGroup(std::vector<Rect2d> rects)
{
	for (int i = 0; i < rects.size(); ++i)
	{
		targets.push_back(Target(rects[i]));
	}
}
double distanceBetweenPoints(cv::Point point1, cv::Point point2) {

	int intX = abs(point1.x - point2.x);
	int intY = abs(point1.y - point2.y);

	return(sqrt(pow(intX, 2) + pow(intY, 2)));
}

void TargetGroup::addTargetToExistingTarget(Target &currentFrameTarget, int &index)
{

	targets[index].rect = currentFrameTarget.rect;

	targets[index].centerPositions.push_back(currentFrameTarget.centerPositions.back());

	targets[index].currentDiagonalSize = currentFrameTarget.currentDiagonalSize;
	targets[index].currentAspectRatio = currentFrameTarget.currentAspectRatio;

	targets[index].stillBeingTracked = true;
	targets[index].currentMatchFoundOrNew = true;
}

void TargetGroup::addNewTarget(Target &currentFrameTarget)
{
	currentFrameTarget.currentMatchFoundOrNew = true;
	targets.push_back(currentFrameTarget);
}



// match current frame targets to existing targets
void TargetGroup::match(TargetGroup &currentFrameTargets) {

	for (auto &existingTarget : targets) {
		existingTarget.currentMatchFoundOrNew = false;
		existingTarget.predictNextPosition();
	}

	for (auto &currentFrameTarget : currentFrameTargets.targets) {

		int indexOfLeastDistance = 0;
		double leastDistance = 100000.0;

		for (unsigned int i = 0; i < targets.size(); i++) {
			Target& existingTarget = targets[i];
			if (existingTarget.stillBeingTracked == true) {
				double distance = distanceBetweenPoints(currentFrameTarget.centerPositions.back(), existingTarget.predictedNextPosition);
				if (distance < leastDistance) {
					leastDistance = distance;
					indexOfLeastDistance = i;
				}
			}
		}

		if (leastDistance < currentFrameTarget.currentDiagonalSize * 1.15) {
			addTargetToExistingTarget(currentFrameTarget, indexOfLeastDistance);
		}
		else {
			addNewTarget(currentFrameTarget);
		}
	}

	for (auto &existingTarget : targets) {
		if (existingTarget.currentMatchFoundOrNew == false) {
			existingTarget.numOfConsecutiveFramesWithoutAMatch++;
		}
		if (existingTarget.numOfConsecutiveFramesWithoutAMatch >= MAX_LOST_FRAMES) {
			existingTarget.stillBeingTracked = false;
		}
	}
}