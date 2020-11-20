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
	Ptr<Tracker> tracker;
	Rect2d trackBox;
	Point point;
	VideoCapture cap;
	Size S;
	int ex;
	VideoWriter video;
public:
	Tracking();
	void TrackingRun();
	void SaveVideo();
};