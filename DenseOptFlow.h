#pragma once

#include <iostream>
#include <fstream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <cstdlib> 
using namespace cv;
using namespace std;

float compareRect(Rect2d rect1, Rect2d rect2);

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
	int step;
	Point update_point;
	Point2f get_flow;
	Point2f print_flow;
	Rect2d tracked_rect;
	int rec_x, rec_y, rec_width, rec_height;
	float sum_delta_x_t, sum_delta_y_t;
	float threshold;
	int y_counter, x_counter, total_counter;
	bool y_dismiss;
	Point2f flow_point;
	Point rec_corner;
	int frame_counter;
	Point2f draw_flow_point;
	int draw_flag;
	float float_x, float_y;
	int approx_x, approx_y;
	String name_og, name_hue, name_sparse;
	int og_counter, hue_counter;
	float correction_factor_x;
	float correction_factor_y;
	float dt;
	float velocity;
	int save_counter, sparse_counter;


public:
	DenseOptFlow();
	void trackDenseFlow();
	void moveROI();
	void showHue(Mat flow);
	void drawAllPoints(Mat flow);
	void darwRectangle();
	void averageFlow(Mat flow);
	void averageFlowTotal(Mat flow);
	void saveToFile();
};

