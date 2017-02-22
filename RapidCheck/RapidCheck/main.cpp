#include <opencv2/opencv.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cstring>
#include <vector>

#define VIDEOFILE "videos/768x576.avi"
#define DETECTION_PERIOD 5
using namespace cv;

int main(int argc, const char * argv[])
{
	// declares all required variables
	Rect2d roi;
	Mat frame;

	// create a tracker object
	Ptr<Tracker> tracker = Tracker::create("KCF");

	// create HOG descriptor
	HOGDescriptor hog;
	hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
	std::vector<Rect> found;
	std::vector<Rect2d> found_filtered;

	// set input video
	VideoCapture cap(VIDEOFILE);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
	
	// get bounding box
	cap >> frame;
	// roi = selectROI("tracker", frame);

	// initialize the tracker
	// tracker->init(frame, roi);

	// perform the tracking process
	printf("Start the tracking process, press ESC to quit.\n");

	int frameCnt = 0;

	while (true){
		// get frame from the video
		cap >> frame;
		// stop the program if no more images
		if (frame.rows == 0 || frame.cols == 0)
			break;

		// implement detection using HOG every n-th frame
		if (frameCnt++ % DETECTION_PERIOD == 0) {

			// reset vectors
			found.clear();
			found_filtered.clear();

			
			hog.detectMultiScale(frame, found, 0, Size(8, 8), Size(32, 32), 1.05, 2);
			size_t i, j;
			for (int i = 0; i<found.size(); i++)
			{
				Rect r = found[i];
				for (j = 0; j<found.size(); j++)
					if (j != i && (r & found[j]) == r)
						break;
				if (j == found.size())
					found_filtered.push_back(r);
			}

			for (i = 0; i<found_filtered.size(); i++)
			{
				Rect r = found_filtered[i];
				r.x += cvRound(r.width*0.1);
				r.width = cvRound(r.width*0.8);
				r.y += cvRound(r.height*0.07);
				r.height = cvRound(r.height*0.8);
				rectangle(frame, r.tl(), r.br(), Scalar(0, 255, 0), 3);
			}

		}

		/*
		// implement tracking during remaining (n-1) frames 
		else {


		}
		Rect
		// update the tracking result
		tracker->update(frame, roi);
		// draw the tracked object
		cv::rectangle(frame, roi, Scalar(255, 0, 0), 2, 1);

		*/
		
		// show image with the tracked object
		imshow("tracker", frame);

		//quit on ESC button
		if (waitKey(100) == 27)break;

	}
	return 0;
}

/*
void hog() {

Mat img;
namedWindow("opencv", CV_WINDOW_AUTOSIZE);
HOGDescriptor hog;
hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

while (true)
{
	cap >> img;
	if (img.empty())
		continue;

	std::vector<Rect> found, found_filtered;
	hog.detectMultiScale(img, found, 0, Size(8, 8), Size(32, 32), 1.05, 2);
	size_t i, j;
	for (int i = 0; i<found.size(); i++)
	{
		Rect r = found[i];
		for (j = 0; j<found.size(); j++)
			if (j != i && (r & found[j]) == r)
				break;
		if (j == found.size())
			found_filtered.push_back(r);
	}

	for (i = 0; i<found_filtered.size(); i++)
	{
		Rect r = found_filtered[i];
		r.x += cvRound(r.width*0.1);
		r.width = cvRound(r.width*0.8);
		r.y += cvRound(r.height*0.07);
		r.height = cvRound(r.height*0.8);
		rectangle(img, r.tl(), r.br(), Scalar(0, 255, 0), 3);
	}

	imshow("opencv", img);
	if (waitKey(30) == 27)
		break;
}
return 0;
}
*/