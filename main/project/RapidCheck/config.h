#ifndef CONFIG_H
#define CONFIG_H

// VideoCapture configuration
const int MAX_FRAMES = 1000;
const int START_FRAME_NUM = 2001;
const int FRAME_STEP = 3;
const int VIDEOID = 3;

const int NUM_OF_COLORS = 64;
const bool DEBUG = false;

const static char* VIDEOFILES[4] = { "videos/street.avi", "videos/tracking.mp4", "videos/demo.mp4", "videos/tracking.mp4" };
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
const int CONTINUOUS_MOTION_COST_THRE_CAR = 100;
const bool RESIZE_DETECTION_AREA = false;
const bool SELECT_DETECTION_RESPONSE = true;

// Trajectory
const double TRAJECTORY_MATCH_COST_THRES_FOR_PEDESTRIAN = 200.0;
const double TRAJECTORY_MATCH_COST_THRES_FOR_CAR = 1500.0;
const double TRAJECTORY_MATCH_SIMILARITY_THRES = 0.55;
const int MAXIMUM_LOST_SEGMENTS = 2;
const double STANDARD_DEVIATION = 2000.0;
const bool INSERT_TRACKING_INTO_DB = false;
const bool INSERT_OBJECT_INFO_INTO_DB = false;

// Object Information
const int NUM_OF_DIRECTIONS = 8;
const int NUM_OF_COLOR_CLASSES = 10;
const int NUM_HUE_GROUP_SIZE = 2;
const static double PI = 3.14159265;

static int totalFrameCount;

#endif