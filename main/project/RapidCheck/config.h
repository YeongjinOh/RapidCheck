#ifndef CONFIG_H
#define CONFIG_H

// VideoCapture configuration
const int MAX_FRAMES = 1000;
const int START_FRAME_NUM = 00;
const int FRAME_STEP = 5;
const int VIDEOID = 1;

const int NUM_OF_COLORS = 64;
const bool DEBUG = false;

const static char* VIDEOFILES[3] = { "videos/street.avi", "videos/tracking.mp4", "videos/demo.mp4" };
const static int videoId = VIDEOID;
const static char* VIDEOFILE = VIDEOFILES[videoId];

// Histogram
const int NUM_OF_HUE_BINS = 16;
const int NUM_OF_SAT_BINS = 6;

// Tracklet
const double MIXTURE_CONSTANT = 0.1;
const int LOW_LEVEL_TRACKLETS = 6;
const int MID_LEVEL_TRACKLETS = 6;
const int NUM_OF_SEGMENTS = (MAX_FRAMES - 1) / LOW_LEVEL_TRACKLETS;
const int NUM_OF_MID_LEVEL_SEGMENTS = NUM_OF_SEGMENTS / MID_LEVEL_TRACKLETS;
const int CONTINUOUS_MOTION_COST_THRE = 30;

// Trajectory
const double TRAJECTORY_MATCH_THRES = 200.0;
const double TRAJECTORY_MATCH_SIMILARITY_THRES = 0.75;
const int MAXIMUM_LOST_SEGMENTS = 10;
const double STANDARD_DEVIATION = 2000.0;

// Object Information
const int NUM_OF_DIRECTIONS = 8;
const int NUM_OF_COLOR_CLASSES = 10;
const int NUM_HUE_GROUP_SIZE = 2;
const static double PI = 3.14159265;

static int totalFrameCount;

#endif