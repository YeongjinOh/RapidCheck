#include "frame.h"

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
vector<Target>& Frame::getTargets()
{
	return targets;
}