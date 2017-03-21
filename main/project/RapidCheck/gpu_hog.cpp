#include "gpu_hog.h"

bool help_showed = false;

static void printHelp()
{
	cout << "Histogram of Oriented Gradients descriptor and detector sample.\n"
		<< "\nUsage: hog_gpu\n"
		<< "  (<image>|--video <vide>|--camera <camera_id>) # frames source\n"
		<< "  or"
		<< "  (--folder <folder_path>) # load images from folder\n"
		<< "  [--svm <file> # load svm file"
		<< "  [--make_gray <true/false>] # convert image to gray one or not\n"
		<< "  [--resize_src <true/false>] # do resize of the source image or not\n"
		<< "  [--width <int>] # resized image width\n"
		<< "  [--height <int>] # resized image height\n"
		<< "  [--hit_threshold <double>] # classifying plane distance threshold (0.0 usually)\n"
		<< "  [--scale <double>] # HOG window scale factor\n"
		<< "  [--nlevels <int>] # max number of HOG window scales\n"
		<< "  [--win_width <int>] # width of the window\n"
		<< "  [--win_stride_width <int>] # distance by OX axis between neighbour wins\n"
		<< "  [--win_stride_height <int>] # distance by OY axis between neighbour wins\n"
		<< "  [--block_width <int>] # width of the block\n"
		<< "  [--block_stride_width <int>] # distance by 0X axis between neighbour blocks\n"
		<< "  [--block_stride_height <int>] # distance by 0Y axis between neighbour blocks\n"
		<< "  [--cell_width <int>] # width of the cell\n"
		<< "  [--nbins <int>] # number of bins\n"
		<< "  [--gr_threshold <int>] # merging similar rects constant\n"
		<< "  [--gamma_correct <int>] # do gamma correction or not\n"
		<< "  [--write_video <bool>] # write video or not\n"
		<< "  [--dst_video <path>] # output video path\n"
		<< "  [--dst_video_fps <double>] # output video fps\n";
	help_showed = true;
}

/*
int main(int argc, char** argv)
{
try
{
Args args;
if (argc < 2)
{
printHelp();
args.camera_id = 0;
args.src_is_camera = true;
}
else
{
args = Args::read(argc, argv);
if (help_showed)
return -1;
}
App app(args);
app.run();
}
catch (const Exception& e) { return cout << "error: " << e.what() << endl, 1; }
catch (const exception& e) { return cout << "error: " << e.what() << endl, 1; }
catch (...) { return cout << "unknown exception" << endl, 1; }
return 0;
}
*/

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


Args Args::read(int argc, char** argv)
{
	Args args;
	for (int i = 1; i < argc; i++)
	{
		if (string(argv[i]) == "--make_gray") args.make_gray = (string(argv[++i]) == "true");
		else if (string(argv[i]) == "--resize_src") args.resize_src = (string(argv[++i]) == "true");
		else if (string(argv[i]) == "--width") args.width = atoi(argv[++i]);
		else if (string(argv[i]) == "--height") args.height = atoi(argv[++i]);
		else if (string(argv[i]) == "--hit_threshold")
		{
			args.hit_threshold = atof(argv[++i]);
			args.hit_threshold_auto = false;
		}
		else if (string(argv[i]) == "--scale") args.scale = atof(argv[++i]);
		else if (string(argv[i]) == "--nlevels") args.nlevels = atoi(argv[++i]);
		else if (string(argv[i]) == "--win_width") args.win_width = atoi(argv[++i]);
		else if (string(argv[i]) == "--win_stride_width") args.win_stride_width = atoi(argv[++i]);
		else if (string(argv[i]) == "--win_stride_height") args.win_stride_height = atoi(argv[++i]);
		else if (string(argv[i]) == "--block_width") args.block_width = atoi(argv[++i]);
		else if (string(argv[i]) == "--block_stride_width") args.block_stride_width = atoi(argv[++i]);
		else if (string(argv[i]) == "--block_stride_height") args.block_stride_height = atoi(argv[++i]);
		else if (string(argv[i]) == "--cell_width") args.cell_width = atoi(argv[++i]);
		else if (string(argv[i]) == "--nbins") args.nbins = atoi(argv[++i]);
		else if (string(argv[i]) == "--gr_threshold") args.gr_threshold = atoi(argv[++i]);
		else if (string(argv[i]) == "--gamma_correct") args.gamma_corr = (string(argv[++i]) == "true");
		else if (string(argv[i]) == "--write_video") args.write_video = (string(argv[++i]) == "true");
		else if (string(argv[i]) == "--dst_video") args.dst_video = argv[++i];
		else if (string(argv[i]) == "--dst_video_fps") args.dst_video_fps = atof(argv[++i]);
		else if (string(argv[i]) == "--help") printHelp();
		else if (string(argv[i]) == "--video") { args.src = argv[++i]; args.src_is_video = true; }
		else if (string(argv[i]) == "--camera") { args.camera_id = atoi(argv[++i]); args.src_is_camera = true; }
		else if (string(argv[i]) == "--folder") { args.src = argv[++i]; args.src_is_folder = true; }
		else if (string(argv[i]) == "--svm") { args.svm = argv[++i]; args.svm_load = true; }
		else if (args.src.empty()) args.src = argv[i];
		else throw runtime_error((string("unknown key: ") + argv[i]));
	}
	return args;
}


App::App(const Args& s)
{
	cv::cuda::printShortCudaDeviceInfo(cv::cuda::getDevice());

	args = s;
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

void App::run()
{
	running = true;
	cv::VideoWriter video_writer;

	Size win_stride(args.win_stride_width, args.win_stride_height);
	Size win_size(args.win_width, args.win_width * 2);
	Size block_size(args.block_width, args.block_width);
	Size block_stride(args.block_stride_width, args.block_stride_height);
	Size cell_size(args.cell_width, args.cell_width);

	cv::Ptr<cv::cuda::HOG> gpu_hog = cv::cuda::HOG::create(win_size, block_size, block_stride, cell_size, args.nbins);
	
	// Create HOG descriptors and detectors here
	Mat detector = gpu_hog->getDefaultPeopleDetector();
	gpu_hog->setSVMDetector(detector);
	
	while (running)
	{
		VideoCapture vc;
		Mat frame;
		vector<String> filenames;

		unsigned int count = 1;

		// set src
		if (args.src_is_video)
		{
			vc.open(args.src.c_str());
			if (!vc.isOpened())
				throw runtime_error(string("can't open video file: " + args.src));
			vc >> frame;
		}

		Mat img_aux, img, img_to_show;
		cuda::GpuMat gpu_img;

		// Iterate over all frames
		while (running && !frame.empty())
		{
			workBegin();

			// Change format of the image (default use_gpu)
			if (make_gray) cvtColor(frame, img_aux, COLOR_BGR2GRAY);
			else if (use_gpu) cvtColor(frame, img_aux, COLOR_BGR2BGRA);
			else frame.copyTo(img_aux);

			// Resize image (default false)
			if (args.resize_src) resize(img_aux, img, Size(args.width, args.height));
			else img = img_aux;
			img_to_show = img;

			vector<Rect> found;

			// Perform HOG classification
			hogWorkBegin();
			if (use_gpu)
			{
				gpu_img.upload(img);
				gpu_hog->setNumLevels(nlevels);
				gpu_hog->setHitThreshold(hit_threshold);
				gpu_hog->setWinStride(win_stride);
				gpu_hog->setScaleFactor(scale);
				gpu_hog->setGroupThreshold(gr_threshold);
				gpu_hog->detectMultiScale(gpu_img, found);
			}
			
			hogWorkEnd();

			// Draw positive classified windows
			for (size_t i = 0; i < found.size(); i++)
			{
				Rect r = found[i];
				rectangle(img_to_show, r.tl(), r.br(), Scalar(0, 255, 0), 3);
			}

			if (use_gpu)
				putText(img_to_show, "Mode: GPU", Point(5, 25), FONT_HERSHEY_SIMPLEX, 1., Scalar(255, 100, 0), 2);
			else
				putText(img_to_show, "Mode: CPU", Point(5, 25), FONT_HERSHEY_SIMPLEX, 1., Scalar(255, 100, 0), 2);
			putText(img_to_show, "FPS HOG: " + hogWorkFps(), Point(5, 65), FONT_HERSHEY_SIMPLEX, 1., Scalar(255, 100, 0), 2);
			putText(img_to_show, "FPS total: " + workFps(), Point(5, 105), FONT_HERSHEY_SIMPLEX, 1., Scalar(255, 100, 0), 2);
			imshow("opencv_gpu_hog", img_to_show);

			vc >> frame;
			
			workEnd();
			handleKey((char)waitKey(3));
		}
	}
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


inline void App::hogWorkBegin() { hog_work_begin = getTickCount(); }

inline void App::hogWorkEnd()
{
	int64 delta = getTickCount() - hog_work_begin;
	double freq = getTickFrequency();
	hog_work_fps = freq / delta;
}

inline string App::hogWorkFps() const
{
	stringstream ss;
	ss << hog_work_fps;
	return ss.str();
}


inline void App::workBegin() { work_begin = getTickCount(); }

inline void App::workEnd()
{
	int64 delta = getTickCount() - work_begin;
	double freq = getTickFrequency();
	work_fps = freq / delta;
}

inline string App::workFps() const
{
	stringstream ss;
	ss << work_fps;
	return ss.str();
}