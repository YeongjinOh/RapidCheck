#RapidCheck

####Smart Video Anaylsis Solution

<p>We're developing video analysis solution to easily find out specific objects using informations such as direction, speed, colors, an so on.</p>
<p>Entire project consists of 3 Modules : Detection engine, Tracking engine, main GUI program.</p>
<br>

##Detection engine 

<p> Detection engine detects people and cars in each frame. </p>
<p> We use deep a learning algorithm to detect objects. Our CNN model is described as follows:</p>

[CNN architecture]


#####Dependencies
<ul>
<li>tensorflow >= 1.0</li>
<li>keras >= 2.0</li>
<li>opencv(python) >= 3.0</li>
<li>matplotlib </li>
<li>pymysql</li>
</ul>
<br>

##Tracking Engine

<p>Tracking engine implements tracking algorithms and analyzes object's informations as the following pipeline.</p>
<ol>
	<li> Read detection responses from database. </li>
	<li> Build Tracklet for short term period. </li>
	<li> Build Trajectory for entire period. </li>
	<li> Extract each object's information such as direction, speed, color.</li>
</ol>

#####Dependencies
<ul>
	<li>opencv(c++) >= 3.0</li>
	<li>mysql</li>
</ul>

#####Installing

<p>This project is based on **Visual Studio 2013**. Our dependecies are set in *cv_x64_debug.props*.
We followed Kusmawan's <a href="https://putuyuwono.wordpress.com/2015/04/23/building-and-installing-opencv-3-0-on-windows-7-64-bit/">Building and Installing OpenCV tutorial</a>.
</p>
<br>

##GUI program

<p>Given analysis results, original video is compressed into a short time video using overlay algorithm. User can choose class(person, car), direction, color to find a specific object.</p>

#####Dependencies

<ol>
<li>OxyPlot 1.0</li>
<li>MaterialSkin 1.0</li>
<li>MySql 6.9.9</li>
<li>CefSharp 47.0</li>
<li>Accord 3.5</li>
<li>Accord.Video 3.4.2</li>
<li>Accord.Video.FFMPEG 3.3</li>
<li>Accord.Video.VFW 3.4.2</li>
<li>Accord.MachineLearning 3.5</li>
</ol>