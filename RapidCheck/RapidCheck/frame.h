#include<opencv2/core/core.hpp>
using namespace cv;
using namespace std;

class Frame 
{
private:
	int frameNumber;
	vector<Rect2d> pedestrians;
public:
	Frame(int num);
	Frame(int num, vector<Rect2d>& pedestrians);
	int getFrameNumbers();
	vector<Rect2d>& getPedestrians();
};