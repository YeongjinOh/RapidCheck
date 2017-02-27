#include<opencv2/core/core.hpp>

class Person
{
public:
	Person(cv::Rect rect);
	
	cv::Rect rect;
	std::vector<cv::Point> centerPositions;

	double currentDiagonalSize;
	double currentAspectRatio;
	bool currentMatchFoundOrNew;
	bool stillBeingTracked;
	int numOfConsecutiveFramesWithoutAMatch;

	cv::Point predictedNextPosition;
	void predictNextPosition(void);
};