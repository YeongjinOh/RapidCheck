#include "main.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;
using namespace rc;
using namespace cv;

void getDir(const char* d, vector<string> & f)
{
	FILE* pipe = NULL;
	string pCmd = "dir /B /S " + string(d);
	char buf[256];

	if (NULL == (pipe = _popen(pCmd.c_str(), "rt")))
	{
		cout << "Shit" << endl;
		return;
	}

	while (!feof(pipe))
	{
		if (fgets(buf, 256, pipe) != NULL)
		{
			f.push_back(string(buf));
		}

	}

	_pclose(pipe);
}



// color extraction tester
void colorExtractor()
{
	// initialize variables for histogram
	// Using 50 bins for hue and 60 for saturation
	int h_bins = NUM_OF_HUE_BINS; int s_bins = NUM_OF_SAT_BINS;
	int histSize[] = { h_bins, s_bins };
	// hue varies from 0 to 179, saturation from 0 to 255
	float h_ranges[] = { 0, 180 };
	float s_ranges[] = { 0, 256 };
	const float* ranges[] = { h_ranges, s_ranges };
	// Use the o-th and 1-st channels
	int channels[] = { 0, 1 };

	int id;
	printf("insert id:");
	scanf("%d", &id);

	//set window
	cv::namedWindow("origin");
	cv::namedWindow("RGB white");
	cv::namedWindow("RGB black");
	//cv::namedWindow("HSV white");
	//cv::namedWindow("HSV black");
	cv::moveWindow("origin", 50, 100);
	cv::moveWindow("RGB white", 500, 100);
	cv::moveWindow("RGB black", 900, 100);
	//cv::moveWindow("HSV white", 500, 500);
	//cv::moveWindow("HSV black", 900, 500);

	int diffB = 30, diffW = 100;
	
	string dir = string("C:\\YJ\\images");
	//string dir = string("C:\\obj\\" + std::to_string(id));
	vector<string> files;
	getDir(dir.c_str(), files);

	for (int i = 0; i < files.size(); i++)
	{
		string file = files[i].substr(0, files[i].size() - 1);
		cout << file << endl;
		Mat img = cv::imread(file);
		MatND hist;
		int w = img.cols, h = img.rows;

		Mat imgRGB, imgHSV;
		int cut = 10;
		Rect rect(cut, cut, img.cols - 2 * cut, img.rows - 2 * cut);
		imgRGB = img(rect);
		cvtColor(imgRGB, imgHSV, cv::COLOR_BGR2HSV);
		Mat imgWhite, imgBlack;

		cv::inRange(imgRGB, Scalar(255 - diffW, 255 - diffW, 255 - diffW, 0), Scalar(255, 255, 255, 0), imgWhite);
		cv::inRange(imgRGB, Scalar(0, 0, 0, 0), Scalar(diffB, diffB, diffB, 0), imgBlack);

		//resize(imgRGB, imgRGB, Size(400, 300));
		//resize(imgWhite, imgWhite, Size(400, 300));
		//resize(imgBlack, imgBlack, Size(400, 300));
		imshow("origin", imgRGB);
		imshow("RGB white", imgWhite);
		imshow("RGB black", imgBlack);

		int area = imgRGB.rows * imgRGB.cols;
		int cntWhite = cv::countNonZero(imgWhite), cntBlack = cv::countNonZero(imgBlack);
		
		/*
		cv::inRange(imgHSV, Scalar(0, 0, 255 - diffW, 0), Scalar(180, 255, 255, 0), imgWhite);
		cv::inRange(imgHSV, Scalar(0, 0, 0, 0), Scalar(180, 255, diffB, 0), imgBlack);
		cntWhite = cv::countNonZero(imgWhite);
		cntBlack = cv::countNonZero(imgBlack);
		//printf("HSV) w:%.2lf b:%.2lf\n", (double)cntWhite / area, (double)cntBlack / area);
		imshow("HSV white", imgWhite);
		imshow("HSV black", imgBlack);
		*/



		calcHist(&imgHSV, 1, channels, Mat(), hist, 2, histSize, ranges, true, false);
		normalize(hist, hist, 0, 1, cv::NORM_MINMAX, -1, Mat());

		float totalHistSum = 0.0;
		vector<float> colorHistSum(hist.rows, 0);
		float wb = 0.0;
		for (int i = 0; i < hist.rows; i++)
		{
			for (int j = 0; j < hist.cols; j++)
			{
				float currentHistValue = hist.at<float>(i, j);
				//printf("%d\t", (int)currentHistValue);
				totalHistSum += currentHistValue;
				if (j == 0)
				{
					wb += currentHistValue;
				}
				else
				{
					colorHistSum[i] += currentHistValue;
				}
			}
		//	cout << endl;
		}
		for (int i = 0; i < colorHistSum.size(); i++)
			printf("%.2lf ", colorHistSum[i] / totalHistSum);
		printf("\twhite or black:%.2lf\t", wb / totalHistSum);
		printf("RGB) w:%.2lf b:%.2lf\n", (double)cntWhite / area, (double)cntBlack / area);
		//cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
		//cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
		cout << endl << endl << endl << endl;
		cvWaitKey(0);
	}
}
