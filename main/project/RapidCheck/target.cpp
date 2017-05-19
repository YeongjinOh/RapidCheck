#include "target.h"
using namespace cv;
Target::Target(Rect rect) :rect(rect)
{
	Point currentCenter;
	currentCenter.x = (rect.x + rect.x + rect.width) / 2;
	currentCenter.y = (rect.y + rect.y + rect.height) / 2;
	centerPositions.push_back(currentCenter);

	found = false;
}
Target::Target(Rect rect, MatND hist) :rect(rect), hist(hist)
{
	Point currentCenter;
	currentCenter.x = (rect.x + rect.x + rect.width) / 2;
	currentCenter.y = (rect.y + rect.y + rect.height) / 2;
	centerPositions.push_back(currentCenter);
	found = false;
}

Target::Target(Rect rect, MatND hist, double whiteRatio, double blackRatio) :rect(rect), hist(hist), whiteRatio(whiteRatio), blackRatio(blackRatio)
{
	Point currentCenter;
	currentCenter.x = (rect.x + rect.x + rect.width) / 2;
	currentCenter.y = (rect.y + rect.y + rect.height) / 2;
	centerPositions.push_back(currentCenter);
	found = false;
}

Point Target::getCenterPoint() {
	return Point(rect.x + rect.width / 2, rect.y + rect.height / 2);
}