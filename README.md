#  RapidCheck

#### Smart Video Anaylsis Solution

 We're developing video analysis solution to easily find out specific objects using informations such as direction, speed, colors, an so on.

 Entire project consists of 3 Modules : 

* Detection Engine 
* Tracking Engine
* Main GUI program



## Detection Engine

 Detection engine detects people and cars in each frame.

 We use a deep learning algorithm to detect objects. Our CNN model is described as follows:

![tag](https://github.com/YeongjinOh/RapidCheck/blob/master/images_md/rcnet_arch.png)

##### Dependencies

* tensorflow >= 1.0
* keras >= 2.0
* opencv-python >= 3.0
* matplotlib
* pymysql



##  Tracking Engine

 Tracking engine implements tracking algorithms and analyzes object's informations as the following pipeline.

![Tracking Pipeline](/images_md/tracking_pipe.png)

```
1. Read detection responses from database
2. Build Tracklet for short term period
3. Build Trajectory for entire period
4. Extract each object's information such as direction, speed, color
```

![Tracket Result](/images_md/tracket_result.png)

##### Dependencies

* opencv(c++) >= 3.0
* mysql



##### Installing

 This project is based on **Visual Studio 2013**. Our dependecies are set in *cv_x64_debug.props*.

 We followed Kusmawan's <a href="https://putuyuwono.wordpress.com/2015/04/23/building-and-installing-opencv-3-0-on-windows-7-64-bit/">Building and Installing OpenCV tutorial</a>.



## Main GUI Program

 Given analysis results, original video is compressed into a short time video using overlay algorithm. User can choose class(person, car), direction, color to find a specific object.

![Screen Shot](/images_md/base.png)

##### Dependencies

* OxyPlot 1.0
* MaterialSkin 1.0
* MySql 6.9.9
* CefSharp 47.0
* Accord.Video.FFMPEG 3.3
* Accord.Video.VFW 3.4.2
* Accord.MachineLearning 3.5




#### References

[1] Zamir AR, Dehghan A, Shah M (2012) Gmcp-tracker: Global multi-object tracking using generalized minimum clique graphs.

[2] Wenhan Luo, Junliang Xing (2014) Multiple Object Tracking: A Literature Review, arXiv:1409.7618

[3] Dehghan A, Shah M , Mubarak  S (2015) GMMCP Tracker: Globally Optimal Generalized Maximum Multi Clique Problem for Multiple Object Tracking 

[4] Joseph Redmon, Santosh Divvala (2015), You Only Look Once: Unified, Real-Time Object Detection, https://pjreddie.com/darknet/yolo/






