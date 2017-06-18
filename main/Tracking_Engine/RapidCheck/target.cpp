#include "target.h"
using namespace cv;
Target::Target(Rect rect) :rect(rect), found(false) { }
Target::Target(Rect rect, MatND hist) : rect(rect), hist(hist), found(false) { }
Target::Target(Rect rect, MatND hist, MatND histColor) : rect(rect), hist(hist), histColor(histColor), found(false) { }

Point Target::getCenterPoint()
{
	return Point(rect.x + rect.width / 2, rect.y + rect.height / 2);
}

Rect Target::getTargetArea()
{
	return rect;
}

