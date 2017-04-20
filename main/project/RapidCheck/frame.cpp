#include "frame.h"
using namespace cv;
Frame::Frame(int num = 0)
{
	frameNumber = num;
}

Frame::Frame(int num, vector<Rect>& pedes)
{
	frameNumber = num;
	pedestrians = pedes;
}

Frame::Frame(int num, vector<Rect>& pedes, vector<MatND> hists)
{
	frameNumber = num;
	pedestrians = pedes;
	for (int i = 0; i < pedes.size() && i < hists.size(); ++i)
	{
		targets.push_back(Target(pedes[i], hists[i]));
	}
}

int Frame::getFrameNumbers()
{
	return frameNumber;
}
vector<Rect>& Frame::getPedestrians()
{
	return pedestrians;
}
Rect & Frame::getPedestrian(int idx)
{
	if (idx < pedestrians.size())
		return pedestrians[idx];
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
	pedestrians.push_back(target.rect);
	return targets.size();
}