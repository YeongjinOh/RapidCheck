#include "main.h"

// color extraction tester
void colorExtractor()
{
	// initialize variables for histogram
	// Using 50 bins for hue and 60 for saturation
	int h_bins = 18; int s_bins = 6;
	int histSize[] = { h_bins, s_bins };
	// hue varies from 0 to 179, saturation from 0 to 255
	float h_ranges[] = { 0, 180 };
	float s_ranges[] = { 0, 256 };
	const float* ranges[] = { h_ranges, s_ranges };
	// Use the o-th and 1-st channels
	int channels[] = { 0, 1 };

	Mat img = cv::imread("C:\\YJ\\personmix.jpg");
	MatND hist;
	int w = img.cols, h = img.rows;
	Rect rect(w / 5, h / 5, 3 * w / 5, 3 * h / 5);
	Mat imgRGB, imgHSV;
	imgRGB = img(rect);
	cvtColor(imgRGB, imgHSV, cv::COLOR_BGR2HSV);
	Mat imgWhite, imgBlack;
	
	cv::inRange(imgRGB, Scalar(225, 225, 225, 0), Scalar(255, 255, 255, 0), imgWhite);
	cv::inRange(imgRGB, Scalar(0, 0, 0, 0), Scalar(30, 30, 30, 0), imgBlack);
	
	int area = imgRGB.rows * imgRGB.cols;
	int cntWhite = cv::countNonZero(imgWhite), cntBlack = cv::countNonZero(imgBlack);
	printf("RGB) w:%.2lf b:%.2lf\n", (double)cntWhite / area, (double)cntBlack / area);
	
	cv::inRange(imgHSV, Scalar(0, 0, 205, 0), Scalar(180, 255, 255, 0), imgWhite);
	cv::inRange(imgHSV, Scalar(0, 0, 0, 0), Scalar(180, 255, 50, 0), imgBlack);
	cntWhite = cv::countNonZero(imgWhite);
	cntBlack = cv::countNonZero(imgBlack);
	printf("HSV) w:%.2lf b:%.2lf\n", (double)cntWhite / area, (double)cntBlack / area);

	imshow("origin", imgRGB);
	imshow("white", imgWhite);
	imshow("black", imgBlack);
	cvWaitKey(0);
	
	
	calcHist(&imgHSV, 1, channels, Mat(), hist, 2, histSize, ranges, true, false);
	normalize(hist, hist, 0, 1, cv::NORM_MINMAX, -1, Mat());

	float totalHistSum = 0.0;
	vector<float> colorHistSum(NUM_OF_COLOR_CLASSES, 0);
	for (int i = 0; i < hist.rows; i++)
	{	
		int currentColorIdx = i / NUM_HUE_GROUP_SIZE;
		for (int j = 0; j < hist.cols; j++)
		{
			float currentHistValue = hist.at<float>(i, j);
			colorHistSum[currentColorIdx] += currentHistValue;
			totalHistSum += currentHistValue;
		}
	}
	for (int i = 0; i < colorHistSum.size(); i++)
		printf("%.2lf ", colorHistSum[i]/totalHistSum);
	cout << endl;

}


int main(int argc, char ** argv)
{
	// set gpu_hog
	Args args;
	args.hit_threshold = 0.9;
	args.hit_threshold_auto = false;
	args.gr_threshold = 6;
	args.scale = 1.05;
	App app(args);

	int operationNum = 1;
	switch (operationNum)
	{
		case 0:
			compareSimilarity(app);
			break;
		case 1:
			buildAndShowTrajectory(app);
			break;
		case 2:
			showTracklet(app);
			break;
		case 3:
			showTrackletClusters(app);
			break;
		case 4:
			showDetection(app);
			break;
		case 5:
			showTrajectory();
			break;
		case 6:
			colorExtractor();
			break;
	}
	return 0;
}