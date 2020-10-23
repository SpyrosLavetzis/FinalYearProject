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
	VideoCapture cap("C:\\Users\\spyro\\Desktop\\test.mp4"); 
	//static const string windowName = "Test Video";
	//VideoCapture cap(0);
	Mat frame; //create frame object
	Ptr<Tracker> tracker;
	tracker = TrackerMOSSE::create(); //create algorith specific tracker
	
	cap.read(frame); //read first frame and wait
	Rect2d trackBox = selectROI(frame, false, false); //let user select RegionOfInterest
	tracker->init(frame, trackBox);	//initialise tracker
	namedWindow("Test", WINDOW_NORMAL);
	resizeWindow("Test", 1080, 740);
	for (;;) {
		
		cap.read(frame);
		if (frame.empty()) { //ensure we aren't stuck in the for loop foreve (after video is done)
			break;
		}
		//cap >> frame;
		/*char im_array[] = "C:\\Users\\spyro\\Desktop\\CodeTest\\1.jpeg";
		
		imwrite(im_array, frame);
		//imshow("Video", frame);
		im_array[32]++;
		cout << im_array << endl;*/

		if (tracker->update(frame, trackBox)) {//update tracking to next frame
			rectangle(frame, trackBox, Scalar(255, 0, 0), 2, 8); //draw rectangle around object
		}
		
		imshow("Test", frame); //show frame with box
		if (waitKey(1) == 27) { //update every 1 ms and break if esc key is pressed
			break;
		}
	}      
	cap.release();


	return 0;
}