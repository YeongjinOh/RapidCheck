#include "App.h"
#include "config.h"

using namespace cv;

Args::Args()
{
	src_is_video = false;
	src_is_camera = false;
	src_is_folder = false;
	svm_load = false;
	camera_id = 0;

	write_video = false;
	dst_video_fps = 24.;

	make_gray = false;

	resize_src = false;
	width = 640;
	height = 480;

	scale = 1.05;
	nlevels = 13;
	gr_threshold = 8;
	hit_threshold = 1.4;
	hit_threshold_auto = true;

	win_width = 48;
	win_stride_width = 8;
	win_stride_height = 8;
	block_width = 16;
	block_stride_width = 8;
	block_stride_height = 8;
	cell_width = 8;
	nbins = 9;

	gamma_corr = true;
}

App::App()
{

	if (DEBUG)
		cv::cuda::printShortCudaDeviceInfo(cv::cuda::getDevice());

	args = Args();
	if (DEBUG)
		cout << "\nControls:\n"
		<< "\tESC - exit\n"
		<< "\tm - change mode GPU <-> CPU\n"
		<< "\tg - convert image to gray or not\n"
		<< "\t1/q - increase/decrease HOG scale\n"
		<< "\t2/w - increase/decrease levels count\n"
		<< "\t3/e - increase/decrease HOG group threshold\n"
		<< "\t4/r - increase/decrease hit threshold\n"
		<< endl;

	use_gpu = true;
	make_gray = args.make_gray;
	scale = args.scale;
	gr_threshold = args.gr_threshold;
	nlevels = args.nlevels;

	if (args.hit_threshold_auto)
		args.hit_threshold = args.win_width == 48 ? 1.4 : 0.;
	hit_threshold = args.hit_threshold;

	gamma_corr = args.gamma_corr;

	if (DEBUG) 
	{
		cout << "Scale: " << scale << endl;
		if (args.resize_src)
			cout << "Resized source: (" << args.width << ", " << args.height << ")\n";
		cout << "Group threshold: " << gr_threshold << endl;
		cout << "Levels number: " << nlevels << endl;
		cout << "Win size: (" << args.win_width << ", " << args.win_width * 2 << ")\n";
		cout << "Win stride: (" << args.win_stride_width << ", " << args.win_stride_height << ")\n";
		cout << "Block size: (" << args.block_width << ", " << args.block_width << ")\n";
		cout << "Block stride: (" << args.block_stride_width << ", " << args.block_stride_height << ")\n";
		cout << "Cell size: (" << args.cell_width << ", " << args.cell_width << ")\n";
		cout << "Bins number: " << args.nbins << endl;
		cout << "Hit threshold: " << hit_threshold << endl;
		cout << "Gamma correction: " << gamma_corr << endl;
		cout << endl;
	}

	// initialize hog
	Size win_stride(args.win_stride_width, args.win_stride_height);
	Size win_size(args.win_width, args.win_width * 2);
	Size block_size(args.block_width, args.block_width);
	Size block_stride(args.block_stride_width, args.block_stride_height);
	Size cell_size(args.cell_width, args.cell_width);
	gpu_hog = cv::cuda::HOG::create(win_size, block_size, block_stride, cell_size, args.nbins);
	gpu_hog->setNumLevels(nlevels);
	gpu_hog->setHitThreshold(hit_threshold);
	gpu_hog->setWinStride(win_stride);
	gpu_hog->setScaleFactor(scale);
	gpu_hog->setGroupThreshold(gr_threshold);

	// Create HOG descriptors and detectors here
	Mat detector = gpu_hog->getDefaultPeopleDetector();
	gpu_hog->setSVMDetector(detector);
}


void App::getHogResults(Mat & frame, vector<Rect> & found)
{
	Mat img;
	cuda::GpuMat gpu_img;
	cvtColor(frame, img, COLOR_BGR2BGRA);
	gpu_img.upload(img);
	gpu_hog->detectMultiScale(gpu_img, found);
}

void App::handleKey(char key)
{
	switch (key)
	{
	case 27:
		running = false;
		break;
	case 'm':
	case 'M':
		use_gpu = !use_gpu;
		cout << "Switched to " << (use_gpu ? "CUDA" : "CPU") << " mode\n";
		break;
	case 'g':
	case 'G':
		make_gray = !make_gray;
		cout << "Convert image to gray: " << (make_gray ? "YES" : "NO") << endl;
		break;
	case '1':
		scale *= 1.05;
		cout << "Scale: " << scale << endl;
		break;
	case 'q':
	case 'Q':
		scale /= 1.05;
		cout << "Scale: " << scale << endl;
		break;
	case '2':
		nlevels++;
		cout << "Levels number: " << nlevels << endl;
		break;
	case 'w':
	case 'W':
		nlevels = max(nlevels - 1, 1);
		cout << "Levels number: " << nlevels << endl;
		break;
	case '3':
		gr_threshold++;
		cout << "Group threshold: " << gr_threshold << endl;
		break;
	case 'e':
	case 'E':
		gr_threshold = max(0, gr_threshold - 1);
		cout << "Group threshold: " << gr_threshold << endl;
		break;
	case '4':
		hit_threshold += 0.25;
		cout << "Hit threshold: " << hit_threshold << endl;
		break;
	case 'r':
	case 'R':
		hit_threshold = max(0.0, hit_threshold - 0.25);
		cout << "Hit threshold: " << hit_threshold << endl;
		break;
	case 'c':
	case 'C':
		gamma_corr = !gamma_corr;
		cout << "Gamma correction: " << gamma_corr << endl;
		break;
	}
}