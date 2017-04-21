#ifndef CONFIG_H
#define CONFIG_H

// VideoCapture configuration
#define MAX_FRAMES 50
#define START_FRAME_NUM 60 // start frame number
#define FRAME_STEP 1
#define VIDEOID 1

#define NUM_OF_COLORS 64
#define DEBUG false

const static char* VIDEOFILES[2] = { "videos/street.avi", "videos/tracking.mp4" };
const static int videoId = VIDEOID;
#define VIDEOFILE VIDEOFILES[videoId]

// Detection And Tracking Method
#define DETECTION_PERIOD 1
#define MAX_TRACKER_NUMS 10
#define MARGIN 50

// Tracklet
#define MIXTURE_CONSTANT 0.1
#define LOW_LEVEL_TRACKLETS 6
#define MID_LEVEL_TRACKLETS 6
#define NUM_OF_SEGMENTS (MAX_FRAMES - 1)/LOW_LEVEL_TRACKLETS
#define NUM_OF_MID_LEVEL_SEGMENTS NUM_OF_SEGMENTS/MID_LEVEL_TRACKLETS
#define CONTINUOUS_MOTION_COST_THRE 30

// Trajectory
#define TRAJECTORY_MATCH_THRES 200
#define MAXIMUM_LOST_SEGMENTS 10
#define STANDARD_DEVIATION 2000

static int totalFrameCount;

#endif