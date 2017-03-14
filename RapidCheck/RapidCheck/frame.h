#include <opencv2/core/core.hpp>
#include "target.h"

using namespace cv;
using namespace std;

class Frame 
{
private:
	int frameNumber;
	vector<Rect> pedestrians;
	vector<Target> targets;
public:
	Frame(int num);
	Frame(int num, vector<Rect>& pedestrians);
	Frame(int num, vector<Rect>& pedestrians, vector<MatND> hists);
	int getFrameNumbers();
	vector<Rect>& getPedestrians();
	vector<Target>& getTargets();
	Target& getTarget(int idx);
	Rect& getPedestrian(int idx);
};