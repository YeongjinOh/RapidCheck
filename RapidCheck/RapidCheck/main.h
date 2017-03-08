#include <opencv2/opencv.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cstring>
#include <vector>

#include "gpu_hog.h"
#include "targetgroup.h"

#define VIDEOFILE "videos/street.avi"
#define DETECTION_PERIOD 1
#define MAX_TRACKER_NUMS 10

#define MARGIN 50

void detectionAndTracking();
void detectionBasedTracking();