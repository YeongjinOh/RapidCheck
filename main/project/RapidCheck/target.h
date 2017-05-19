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
public:
	Target(){};
	Target(Rect rect);
	Target(Rect rect, MatND hist);
	Target(Rect rect, MatND hist, double whiteRatio, double blackRatio);
	
	Rect rect;
	std::vector<Point> centerPositions;
	double blackRatio, whiteRatio;
	bool found;
	Point getCenterPoint();

	// Histogram for matching
	MatND hist;
};


#endif