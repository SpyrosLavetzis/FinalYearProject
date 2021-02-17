#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
using namespace cv;
using namespace std;
int main()
{

    VideoCapture capture;
    capture.open("C:\\Users\\spyro\\Desktop\\Ants\\ZoomedSlow.mp4");
    if (!capture.isOpened()) {
        //error in opening the video input
        cerr << "Unable to open file!" << endl;
        return 0;
    }
    Mat frame1, prvs, frame;
    capture.read(frame);
    Rect2d box = selectROI("test", frame, false, false);
    frame(box).copyTo(frame1);
    cvtColor(frame1, prvs, COLOR_BGR2GRAY);
    while (true) {
        Mat frame2, next, box_frame;
        capture.read(box_frame);
        double timer = (double)getTickCount();
        box_frame(box).copyTo(frame2);
        if (frame2.empty())
            break;
        cvtColor(frame2, next, COLOR_BGR2GRAY);
        Mat flow(prvs.size(), CV_32FC2);
        calcOpticalFlowFarneback(prvs, next, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
        // visualization
        Mat flow_parts[2];
        split(flow, flow_parts);
        Mat magnitude, angle, magn_norm;
        cartToPolar(flow_parts[0], flow_parts[1], magnitude, angle, true);
        normalize(magnitude, magn_norm, 0.0f, 1.0f, NORM_MINMAX);
        angle *= ((1.f / 360.f) * (180.f / 255.f));
        //build hsv image
        Mat _hsv[3], hsv, hsv8, bgr;
        _hsv[0] = angle;
        _hsv[1] = Mat::ones(angle.size(), CV_32F);
        _hsv[2] = magn_norm;
        merge(_hsv, 3, hsv);
        hsv.convertTo(hsv8, CV_8U, 255.0);
        cvtColor(hsv8, bgr, COLOR_HSV2BGR);
        double freq = getTickFrequency() / ((double)getTickCount() - timer);
        putText(bgr, to_string(freq), Point(100, 80), FONT_HERSHEY_PLAIN, 1.5, Scalar(0, 0, 255), 2);
        imshow("frame2", bgr);
        int keyboard = waitKey(30);
        if (keyboard == 'q' || keyboard == 27)
            break;
        prvs = next;
    }
}