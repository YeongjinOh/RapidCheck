#ifndef MAIN_H
#define MAIN_H

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
#include "frame.h"

#define VIDEOFILE "videos/street.avi"
//#define VIDEOFILE "videos/tracking.mp4"
#define DETECTION_PERIOD 1
#define MAX_TRACKER_NUMS 10
#define MARGIN 50

// set basic colors
#define WHITE Scalar(255,255,255)
#define BLUE Scalar(255,0,0)
#define GREEN Scalar(0,255,0)
#define RED Scalar(0,0,255)

void detectionAndTracking(App app);
void detectionBasedTracking(App app);
void showTracklet(App app);

void buildTrajectory(App app);

#endif