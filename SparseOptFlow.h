


#pragma once

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
using namespace cv;
using namespace std;

class SparseOptFlow {

private:
	VideoCapture capture;
	// Create some random colors
	vector<Scalar> colors;
	RNG rng;
	Mat old_frame;
	Mat	old_gray;
	vector<Point2f> p1;
	Mat mask;
	double timer;
	Mat frame;
	Mat	frame_gray;
	vector<uchar> status;
	vector<float> err;
	TermCriteria criteria;
	vector<Point2f> good_new;
	Mat img;
	double freq;
	int keyboard;
	int frame_counter;
	int save_counter;
	//Point point;

public:
	vector<Point2f> p0;
	// constructor
	SparseOptFlow();
	void trackOptFlow();
	void playVideo();
};

