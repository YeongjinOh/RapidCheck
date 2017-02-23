#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main()
{
	cv::Mat image; // create an empty image

	image = cv::imread("images/puppy.jpg"); // read an input image

	// define the window (optional)
	cv::namedWindow("Original Image");
	// show the image
	cv::imshow("Original Image", image);

	cv::waitKey(0); // 0 to indefinitely wait for a key pressed
	// specifying a positive value will wait for
	// the given amount of msec

	return 0;
}