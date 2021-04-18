
#ifndef RUN_GT



#include <iostream>
#include <stdio.h>
#include <opencv2\videoio.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2\core.hpp>
#include <opencv2\tracking.hpp>
#include "DenseOptFlow.h"
#include "SparseOptFlow.h"

int main()
{

    DenseOptFlow track;
    track.trackDenseFlow();

	return 0;
}

//Tracking API

/*#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include <string.h>

using namespace cv;
using namespace std;

// Convert to string
#define SSTR( x ) static_cast< std::ostringstream & >( \
( std::ostringstream() << std::dec << x ) ).str()

int main(int argc, char** argv)
{
    int save_counter = 0;
    String name_sparse;
    int sparse_counter = 0;
    // List of tracker types in OpenCV 3.4.1
    string trackerTypes[8] = { "BOOSTING", "MIL", "KCF", "TLD","MEDIANFLOW", "GOTURN", "MOSSE", "CSRT" };
    // vector <string> trackerTypes(types, std::end(types));

    // Create a tracker
    string trackerType = trackerTypes[7];

    Ptr<Tracker> tracker;

    #if (CV_MINOR_VERSION < 3)
    {
        tracker = Tracker::create(trackerType);
    }
    #else
    {
        if (trackerType == "BOOSTING")
            tracker = TrackerBoosting::create();
        if (trackerType == "MIL")
            tracker = TrackerMIL::create();
        if (trackerType == "KCF")
            tracker = TrackerKCF::create();
        if (trackerType == "TLD")
            tracker = TrackerTLD::create();
        if (trackerType == "MEDIANFLOW")
            tracker = TrackerMedianFlow::create();
        if (trackerType == "GOTURN")
            tracker = TrackerGOTURN::create();
        if (trackerType == "MOSSE")
            tracker = TrackerMOSSE::create();
        if (trackerType == "CSRT")
            tracker = TrackerCSRT::create();
    }
    #endif
    // Read video
    VideoCapture video("C:\\Users\\spyro\\Desktop\\Ants\\ZoomedFast.mp4");

    // Exit if video is not opened
    if (!video.isOpened())
    {
        cout << "Could not read video file" << endl;
        return 1;
    }

    // Read first frame 
    Mat frame;
    bool ok = video.read(frame);

    // Define initial bounding box 
    Rect2d bbox(287, 23, 86, 320);

    // Uncomment the line below to select a different bounding box 
    bbox = selectROI(frame, false); 
    // Display bounding box. 
    rectangle(frame, bbox, Scalar(255, 0, 0), 2, 1);

    imshow("Tracking", frame);
    tracker->init(frame, bbox);

    while (video.read(frame))
    {
        save_counter += 1;
        // Start timer
        double timer = (double)getTickCount();

        // Update the tracking result
        bool ok = tracker->update(frame, bbox);

        // Calculate Frames per second (FPS)
        double fps = getTickFrequency() / ((double)getTickCount() - timer);

        if (ok)
        {
            // Tracking success : Draw the tracked object
            rectangle(frame, bbox, Scalar(255, 0, 0), 6, 1);
        }
        else
        {
            // Tracking failure detected.
            putText(frame, "Tracking failure detected", Point(100, 80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 2);
        }

        // Display tracker type on frame
        putText(frame, trackerType + " Tracker", Point(100, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);

        // Display FPS on frame
        putText(frame, "FPS : " + to_string(fps), Point(100, 50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);
        if (save_counter == 10) {
            name_sparse = "C:\\Users\\spyro\\Desktop\\Ants\\FastCompare\\FastCSRT" + to_string(sparse_counter) + ".png";
            imwrite(name_sparse, frame);
            sparse_counter += 1;
            save_counter = 0;
        }
        // Display frame.
        imshow("Tracking", frame);

        // Exit if ESC pressed.
        int k = waitKey(1);
        if (k == 27)
        {
            break;
        }

    }
}*/

// Background Subtraction

/*#include <iostream>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
using namespace cv;
using namespace std;
int main()
{
    String name_sparse;
    int sparse_counter = 0;
    //create Background Subtractor objects
    Ptr<BackgroundSubtractor> pBackSub;

    pBackSub = createBackgroundSubtractorKNN();
    VideoCapture capture("C:\\Users\\spyro\\Desktop\\Ants\\ZoomedSlow.mp4");
    if (!capture.isOpened()) {
        //error in opening the video input
        cerr << "Unable to open: " << endl;
        return 0;
    }
    Mat frame, fgMask;
    while (true) {
        capture >> frame;
        if (frame.empty())
            break;
        //update the background model
        pBackSub->apply(frame, fgMask);
        //get the frame number and write it on the current frame
        rectangle(frame, cv::Point(10, 2), cv::Point(100, 20),
            cv::Scalar(255, 255, 255), -1);
        stringstream ss;
        ss << capture.get(CAP_PROP_POS_FRAMES);
        string frameNumberString = ss.str();
        putText(frame, frameNumberString.c_str(), cv::Point(15, 15),
            FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
        //show the current frame and the fg masks
        Rect box = boundingRect(fgMask);
        rectangle(frame, box, Scalar(255, 0, 0), 2, 8);
        imshow("Frame", frame);
        imshow("FG Mask", fgMask);
        name_sparse = "C:\\Users\\spyro\\Desktop\\Ants\\BackSub" + to_string(sparse_counter) + ".png";
        imwrite(name_sparse, fgMask);
        sparse_counter += 1;
        //get the input from the keyboard
        int keyboard = waitKey(1);
        if (keyboard == 'q' || keyboard == 27)
            break;
    }
    return 0;
}*/

#endif // !RUN_GT