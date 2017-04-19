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
#include "config.h"


// set basic colors
#define WHITE Scalar(255,255,255)
#define BLACK Scalar(0,0,0)
#define BLUE Scalar(255,0,0)
#define GREEN Scalar(0,255,0)
#define RED Scalar(0,0,255)

void detectionAndTracking(App app);
void detectionBasedTracking(App app);
void showTracklet(App app);

void buildTrajectory(App app);
void compareSimilarity(App app);

#endif