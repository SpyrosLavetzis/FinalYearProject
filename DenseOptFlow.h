#pragma once

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
using namespace cv;
using namespace std;

class DenseOptFlow {
private:
	VideoCapture capture;
	bool break_flag;
	Mat frame1, prvs, frame;
	Rect2d box;
	//Mat frame2, next, box_frame;
	Mat box_frame, frame2;
	double timer, freq;
	Mat flow_parts[2];
	Mat magnitude, angle, magn_norm;
	Mat _hsv[3], hsv, hsv8, bgr;
	int keyboard;

public:
	DenseOptFlow();
	void trackDenseFlow();
	void moveROI();

};
