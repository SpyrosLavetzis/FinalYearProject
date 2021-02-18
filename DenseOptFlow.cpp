#include"DenseOptFlow.h"


DenseOptFlow::DenseOptFlow()
{
    capture.open("C:\\Users\\spyro\\Desktop\\Ants\\ZoomedSlow.mp4");
    break_flag = 0;
    //capture.open(0);
    if (!capture.isOpened()) {
        //error in opening the video input
        cerr << "Unable to open file!" << endl;
        break_flag = 1;
    }
    capture.read(frame);
    box = selectROI("test", frame, false, false);
    frame(box).copyTo(frame1);
    cvtColor(frame1, prvs, COLOR_BGR2GRAY);
}

void DenseOptFlow::trackDenseFlow()
{

    while (true) {
        if (break_flag == 1) {
            break;
        }
        //needs to be reset every time
        Mat next;
        capture.read(box_frame);
        timer = (double)getTickCount();
        box_frame(box).copyTo(frame2);
        if (frame2.empty())
            break;
        cvtColor(frame2, next, COLOR_BGR2GRAY);
        Mat flow(prvs.size(), CV_32FC2);
        calcOpticalFlowFarneback(prvs, next, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
        // visualization
        split(flow, flow_parts);
        cartToPolar(flow_parts[0], flow_parts[1], magnitude, angle, true);
        normalize(magnitude, magn_norm, 0.0f, 1.0f, NORM_MINMAX);
        angle *= ((1.f / 360.f) * (180.f / 255.f));
        //build hsv image
        _hsv[0] = angle;
        _hsv[1] = Mat::ones(angle.size(), CV_32F);
        _hsv[2] = magn_norm;
        merge(_hsv, 3, hsv);
        hsv.convertTo(hsv8, CV_8U, 255.0);
        cvtColor(hsv8, bgr, COLOR_HSV2BGR);
        freq = getTickFrequency() / ((double)getTickCount() - timer);
        putText(bgr, to_string(freq), Point(100, 80), FONT_HERSHEY_PLAIN, 1.5, Scalar(0, 0, 255), 2);
        imshow("frame2", bgr);
        keyboard = waitKey(30);
        if (keyboard == 'q' || keyboard == 27)
            break;
        prvs = next;
       // flow_parts->clear;
    }
}

void DenseOptFlow::moveROI() {

}