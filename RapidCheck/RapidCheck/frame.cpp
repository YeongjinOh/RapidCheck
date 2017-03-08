#include "frame.h"

Frame::Frame(int num = 0)
{
	frameNumber = num;
}

Frame::Frame(int num, vector<Rect2d>& pedes)
{
	frameNumber = num;
	pedestrians = pedes;
}

int Frame::getFrameNumbers()
{
	return frameNumber;
}
vector<Rect2d>& Frame::getPedestrians()
{
	return pedestrians;
}