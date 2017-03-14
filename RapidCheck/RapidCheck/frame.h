#include<opencv2/core/core.hpp>
using namespace cv;
using namespace std;

class Frame 
{
private:
	int frameNumber;
	vector<Rect> pedestrians;
public:
	Frame(int num);
	Frame(int num, vector<Rect>& pedestrians);
	int getFrameNumbers();
	vector<Rect>& getPedestrians();
};