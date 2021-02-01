#pragma once //same as ifndef
#include <iostream>
#include <stdio.h>
#include <opencv2\videoio.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2\core.hpp>
#include <opencv2\tracking.hpp>
#include <string.h>



using namespace cv;

class Tracking {
private:
	Mat frame; //create frame object
	Mat imposter;
	Mat img1;
	Mat img2;
	Mat grayscale;
	Mat mask;
	Ptr<Tracker> tracker;
	Rect2d track_box;
	Rect box;
	Point point;
	VideoCapture cap;
	Size S;
	int ex;
	double timer;
	double freq;

	VideoWriter video;
	int frame_tracker;
	void MOSSEtoORB(Mat& img1, Mat& img2, Mat frame);
public:
	Tracking();
	void TrackingRun();
	void SaveVideo();
};