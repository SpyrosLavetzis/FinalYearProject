//Tracking API
//Code taken from https://learnopencv.com/object-tracking-using-opencv-cpp-python/ and modified by Spyros lavetzis to get appropriate metrics

#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include <string.h>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

// Convert to string
#define SSTR( x ) static_cast< std::ostringstream & >( \
( std::ostringstream() << std::dec << x ) ).str()

float compareRect(Rect2d rect1, Rect2d rect2);

#ifdef API

int main(int argc, char** argv)
{
    #ifdef COMP_RECT
    ifstream ifile("C:\\Users\\spyro\\Desktop\\Ants\\ZoomedSlow.txt");
    float metric = 0;
    float metric_total = 0;
    long long metric_counter = 0;
    int tp = 0;
    int fp = 0;
    int fn = 0;
    float precision, recall;
    #endif // COMP_RECT

    int save_counter = 0;
    String name_sparse;
    int sparse_counter = 0;
    // List of tracker types in OpenCV 3.4.1
    string trackerTypes[8] = { "BOOSTING", "MIL", "KCF", "TLD","MEDIANFLOW", "GOTURN", "MOSSE", "CSRT" };
    // vector <string> trackerTypes(types, std::end(types));

    // Create a tracker
    string trackerType = trackerTypes[2];

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
    VideoCapture video("C:\\Users\\spyro\\Desktop\\Ants\\ZoomedSlow.mp4");

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
            rectangle(frame, bbox, Scalar(0, 255, 0), 5, 1);
            #ifdef COMP_RECT
            int truth_x, truth_y;
            ifile >> truth_x >> truth_y;
            Rect2d truth_rect = Rect2d((truth_x - bbox.width / 2), (truth_y - bbox.height / 2), bbox.width, bbox.height);
            metric = compareRect(bbox, truth_rect);
            metric_total += metric;
            if (metric >= 0.5) {
                //True Positive
                tp += 1;
            }
            else if (metric == 0)
                //False Negative
                fn += 1;
            else
                //False Positive
                fp += 1;
            //cout << "metric:" << metric << endl;  
            rectangle(frame, truth_rect, Scalar(255, 0, 0), 5, 1);
            //rectangle(box_frame, tracked_rect, Scalar(0, 255, 0), 5, 1);
            metric_counter += 1;
            #endif // COMP_RECT
        }
        else
        {
            // Tracking failure detected.
            #ifdef COMP_RECT
            metric_counter += 1;
            fn += 1;
            #endif // COMP_RECT
            putText(frame, "Tracking failure detected", Point(100, 80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 2);
        }

        // Display tracker type on frame
        putText(frame, trackerType + " Tracker", Point(100, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);

        // Display FPS on frame
        putText(frame, "FPS : " + to_string(fps), Point(100, 50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);
        /*if (save_counter == 10) {
            name_sparse = "C:\\Users\\spyro\\Desktop\\Ants\\Metrics\\KCFMetric" + to_string(sparse_counter) + ".png";
            imwrite(name_sparse, frame);
            sparse_counter += 1;
            save_counter = 0;
        }*/
        // Display frame.
        imshow("Tracking", frame);

        // Exit if ESC pressed.
        int k = waitKey(1);
        if (k == 27)
        {
            break;
        }

    }
    metric_counter = metric_counter - 4;
    metric_total = metric_total / metric_counter;
    precision = (float)tp / (tp + fp);
    recall = (float)tp / (tp + fn);
    cout << "KCF metric: " << (int)(metric_total * 100) << "%" << endl;
    cout << "Precision: " << precision << endl;
    cout << "Recall: " << recall << endl;
}
#endif // API
