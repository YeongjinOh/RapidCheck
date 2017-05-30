#include "frame.h"
using namespace cv;
Frame::Frame(int num = 0) : frameNumber(num) { }



Frame::Frame(int num, vector<Rect>& pedes, vector<MatND> hists) : frameNumber(num)
{
	for (int i = 0; i < pedes.size() && i < hists.size(); ++i)
	{
		targets.push_back(Target(pedes[i], hists[i]));
	}
}
Frame::Frame(int num, vector<Target>& targets) : frameNumber(num), targets(targets) { }

int Frame::getFrameNumbers()
{
	return frameNumber;
}
vector<Rect> Frame::getPedestrians()
{
	vector<Rect> pedestrians;
	for (int i = 0; i < targets.size(); i++)
	{
		pedestrians.push_back(targets[i].getTargetArea());
	}
	return pedestrians;
}
Rect & Frame::getPedestrian(int idx)
{
	if (idx < targets.size())
		return targets[idx].getTargetArea();
	// TODO : Error handling
	printf("***** Error : pedestrians does not have idx elements *****");
	return Rect();
}
vector<Target>& Frame::getTargets()
{
	return targets;
}
Target & Frame::getTarget(int idx)
{
	if (idx < targets.size())
		return targets[idx];
	// TODO : Error handling
	printf("***** Error : targets does not have idx elements *****");
	return Target();
}


// return target size
int Frame::addTarget(Target target)
{
	if (targets.size() > 10)
		return targets.size();
	targets.push_back(target);
	return targets.size();
}