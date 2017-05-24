#include <opencv2/core/core.hpp>
#include "target.h"

using namespace std;
using cv::Rect;
using cv::MatND;

class Frame 
{
private:
	int frameNumber;
	vector<Target> targets;
public:
	Frame(int num);
	Frame(int num, vector<Rect>& pedestrians, vector<MatND> hists);
	Frame(int num, vector<Target>& targets);
	int getFrameNumbers();
	vector<Rect> getPedestrians();
	vector<Target>& getTargets();
	Target& getTarget(int idx);
	Rect& getPedestrian(int idx);
	int addTarget(Target target);
};