#ifndef TARGET_H
#define TARGET_H

#include<opencv2/core/core.hpp>
using cv::Point;
using cv::Point2d;
using cv::Rect2d;
using cv::Rect;
using cv::MatND;

class Target
{
private:
	Rect rect;
public:
	Target(){};
	Target(Rect rect);
	Target(Rect rect, MatND hist);
	Target(Rect rect, MatND hist, MatND histColor);
	
	bool found;
	Point getCenterPoint();
	Rect getTargetArea();

	// Histogram for matching
	MatND hist, histColor;
};


#endif