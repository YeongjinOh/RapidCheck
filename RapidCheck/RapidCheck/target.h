#include<opencv2/core/core.hpp>
using namespace cv;

class Target
{
public:
	Target(Rect rect);
	
	Rect rect;
	std::vector<Point> centerPositions;

	double currentDiagonalSize;
	double currentAspectRatio;
	bool currentMatchFoundOrNew;
	bool stillBeingTracked;
	int numOfConsecutiveFramesWithoutAMatch;

	Point predictedNextPosition;
	void predictNextPosition(void);
};