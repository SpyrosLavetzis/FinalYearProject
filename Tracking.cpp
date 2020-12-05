#include "Tracking.h"
using namespace std;
//VideoCapture cap("C:\\Users\\spyro\\Desktop\\RealAnt1.MOV");
Tracking::Tracking() {
	cap.open("C:\\Users\\spyro\\Desktop\\Ants\\RealAnt1.MOV"); // open video file
	//cap.open("C:\\Users\\spyro\\Desktop\\test.mp4"); 
		//static const string windowName = "Test Video";
	//cap.open(0);
	//tracker = TrackerGOTURN::create(); //create algorithm specific tracker
	tracker = TrackerMOSSE::create();
	//cap.read(frame);
	namedWindow("Test", WINDOW_NORMAL);
	resizeWindow("Test", 1000, 700);
	//trackBox = selectROI("Test", frame, false, false);
	//tracker->init(frame, trackBox);	//initialise tracker
	template_img = Mat();
	frame_tracker = 0;
}

void Tracking::SaveVideo() {
	S = Size((int)cap.get(CAP_PROP_FRAME_WIDTH),    // Acquire input size
		(int)cap.get(CAP_PROP_FRAME_HEIGHT));
	ex = static_cast<int>(cap.get(CAP_PROP_FOURCC));
	video.open("C:\\Users\\spyro\\Desktop\\tracker3.MOV", ex, cap.get(CAP_PROP_FPS), S, true);
	//this needs to be called for every frame.
	//video.write(frame);
}

void Tracking::TrackingRun() {
	
	for (;;) {

		cap.read(frame);
		if (frame.empty()) { //ensure we aren't stuck in the for loop foreve (after video is done)
			break;
		}
		char c = 'i';
		if ((waitKey(1) & 0xff) == int(c)) { //if i is pressed select ROI
			tracker = TrackerMOSSE::create();
			track_box = selectROI("Test", frame, false, false);
			tracker->init(frame, track_box);	//initialise tracker
			//Mat(template_img, trackBox);
			//const Rect box = Rect(trackBox);
			//Rect* const box_ptr = &box;
			//Mat* const template_prt = &template_img;
			//Mat(template_img, box);
			
			frame(track_box).copyTo(template_img);
			//template_img.resize(trackBox.size());
			imshow("Template", template_img);
			//imposter = template_img.clone();
		}
		

		if (tracker->update(frame, track_box)) {//update tracking to next frame
			rectangle(frame, track_box, Scalar(255, 0, 0), 2, 8); //draw rectangle around object
			frame_tracker++;
		}
		if (frame_tracker == 10) {
			frame_tracker = 0;
			Rect box((track_box.x - 100), (track_box.y - 100), (track_box.width + 200), (track_box.height + 200));
			frame(box).copyTo(img);

		}
		/*else { //if algo fails redraw
			cout << "here" << endl; 
			tracker->clear();
			tracker = TrackerMOSSE::create();
			//trackBox = selectROI("Test", frame, false, false);
			tracker->init(frame, trackBox);
		}*/

		/*Point point = trackBox.tl(); //bottom right corner of ROI
		
		//no matter the ration, br end of frame at 630-640 for pc cam
		cout << point << << endl;*/
		//video.write(frame);
		imshow("Test", frame); //show frame with box
		//imshow("Template", template_img);
	
		if (waitKey(1) == 27) { //update every 1 ms and break if esc key is pressed
			break;
		}
	}
	cap.release();
}

