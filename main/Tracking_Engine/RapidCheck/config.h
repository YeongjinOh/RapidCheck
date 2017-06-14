#ifndef CONFIG_H
#define CONFIG_H

// VideoCapture configuration

const int NUM_OF_COLORS = 64;
const bool DEBUG = false;
const bool USE_PEDESTRIANS_ONLY = true;


const int CLASS_ID_CAR = 0;
const int CLASS_ID_PEDESTRIAN = 1;

// Histogram
const int NUM_OF_HUE_BINS = 16;
const int NUM_OF_SAT_BINS = 6;

// Tracklet
const double MIXTURE_CONSTANT = 0.1;
const int LOW_LEVEL_TRACKLETS = 6;
const int MID_LEVEL_TRACKLETS = 6;
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
const int WHITE_DIFF_RANGE = 80;
const int BLACK_DIFF_RANGE = 50;
const int NUM_OF_DIRECTIONS = 8;
const int NUM_OF_COLOR_CLASSES = 19;
const static double PI = 3.14159265;

static int totalFrameCount;

#endif