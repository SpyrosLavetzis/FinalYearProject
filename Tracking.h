#pragma once //same as ifndef
#include <iostream>
#include <stdio.h>
#include <opencv2\videoio.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2\core.hpp>
#include <opencv2\tracking.hpp>



using namespace cv;

class Tracking {
private:
	Mat frame; //create frame object
	Mat imposter;
	Mat template_img;
	Mat img;
	Ptr<Tracker> tracker;
	Rect2d track_box;
	Rect box;
	
	Point point;
	VideoCapture cap;
	Size S;
	int ex;
	VideoWriter video;
	int frame_tracker;
public:
	Tracking();
	void TrackingRun();
	void SaveVideo();
};