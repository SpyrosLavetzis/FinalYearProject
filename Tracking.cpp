#include "Tracking.h"
using namespace std;
//VideoCapture cap("C:\\Users\\spyro\\Desktop\\RealAnt1.MOV");
Tracking::Tracking() {
	cap.open("C:\\Users\\spyro\\Desktop\\Ants\\FakeAnt1.MOV"); // open video file
	//cap.open("C:\\Users\\spyro\\Desktop\\test.mp4"); 
		//static const string windowName = "Test Video";
	//cap.open(0);
	//tracker = TrackerGOTURN::create(); //create algorithm specific tracker
	tracker = TrackerMOSSE::create();
	cap.read(frame);
	namedWindow("Test", WINDOW_NORMAL);
	resizeWindow("Test", 1000, 700);
	trackBox = selectROI("Test", frame, false, false);
	tracker->init(frame, trackBox);	//initialise tracker
}

void Tracking::SaveVideo() {
	S = Size((int)cap.get(CAP_PROP_FRAME_WIDTH),    // Acquire input size
		(int)cap.get(CAP_PROP_FRAME_HEIGHT));
	ex = static_cast<int>(cap.get(CAP_PROP_FOURCC));
	video.open("C:\\Users\\spyro\\Desktop\\tracker3.MOV", ex, cap.get(CAP_PROP_FPS), S, true);
	//this needs to be called for every frame.
	video.write(frame);
}

void Tracking::TrackingRun() {
	
	for (;;) {

		cap.read(frame);
		if (frame.empty()) { //ensure we aren't stuck in the for loop foreve (after video is done)
			break;
		}
		
		if (tracker->update(frame, trackBox)) {//update tracking to next frame
			rectangle(frame, trackBox, Scalar(255, 0, 0), 2, 8); //draw rectangle around object
		}
		else { //if algo fails redraw
			cout << "here" << endl; 
			tracker->clear();
			tracker = TrackerMOSSE::create();
			//trackBox = selectROI("Test", frame, false, false);
			tracker->init(frame, trackBox);
		}

		/*Point point = trackBox.br(); //bottom right corner of ROI
		//no matter the ration, br end of frame at 630-640 for pc cam
		cout << point << endl;*/

		imshow("Test", frame); //show frame with box
	
		if (waitKey(1) == 27) { //update every 1 ms and break if esc key is pressed
			break;
		}
	}
	cap.release();
}

