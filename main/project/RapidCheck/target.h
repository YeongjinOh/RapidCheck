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
	
	Rect rect;
	std::vector<Point> centerPositions;

	bool found;


	double currentDiagonalSize;
	double currentAspectRatio;
	bool currentMatchFoundOrNew;
	bool stillBeingTracked;
	int numOfConsecutiveFramesWithoutAMatch;

	Point predictedNextPosition;
	void predictNextPosition(void);
	Point getCenterPoint();

	// Histogram for matching
	MatND hist;
};


#endif