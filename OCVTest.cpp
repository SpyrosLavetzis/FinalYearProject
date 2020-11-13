#include <iostream>
#include <stdio.h>
#include <opencv2\videoio.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2\core.hpp>
#include <opencv2\tracking.hpp>


int main() {
	using namespace cv;
	using namespace std;
	

	//VideoCapture cap("C:\\Users\\spyro\\Desktop\\Ant.mp4"); // open video file
	//VideoCapture cap("C:\\Users\\spyro\\Desktop\\test.mp4"); 
	//static const string windowName = "Test Video";
	VideoCapture cap(0);
	Mat frame; //create frame object
	Ptr<Tracker> tracker;
	tracker = TrackerMOSSE::create(); //create algorith specific tracker
	
	cap.read(frame); //read first frame and wait
	Rect2d trackBox = selectROI(frame, false, false); //let user select RegionOfInterest
	tracker->init(frame, trackBox);	//initialise tracker
	namedWindow("Test", WINDOW_NORMAL);
	resizeWindow("Test", 1000, 700);
	for (;;) {
		
		cap.read(frame);
		if (frame.empty()) { //ensure we aren't stuck in the for loop foreve (after video is done)
			break;
		}

		if (tracker->update(frame, trackBox)) {//update tracking to next frame
			rectangle(frame, trackBox, Scalar(255, 0, 0), 2, 8); //draw rectangle around object
		}
		Point point = trackBox.br(); //bottom right corner of ROI
		//no matter the ration, br end of frame at 630-640
		cout << point << endl;
		imshow("Test", frame); //show frame with box
		if (waitKey(1) == 27) { //update every 1 ms and break if esc key is pressed
			break;
		}
	}
	cap.release();


	return 0;
}