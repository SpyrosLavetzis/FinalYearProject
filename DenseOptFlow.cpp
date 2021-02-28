#include"DenseOptFlow.h"


DenseOptFlow::DenseOptFlow()
{
    capture.open("C:\\Users\\spyro\\Desktop\\Ants\\ZoomedFast.mp4");
    break_flag = 0;
    //capture.open(0);
    if (!capture.isOpened()) {
        //error in opening the video input
        cerr << "Unable to open file!" << endl;
        break_flag = 1;
    }
    capture.read(frame);
    //cout << frame.at<int>(0, 0) << endl;
    box = selectROI("test", frame, false, false);
    frame(box).copyTo(frame1);
    cvtColor(frame1, prvs, COLOR_BGR2GRAY);
    next_x = 0;
    next_y = 0;
    
    
}

void DenseOptFlow::trackDenseFlow()
{

    while (true) {
        delta_x = 0;
        delta_y = 0;
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
        
        //cout << "Flow:" << flow_point << endl;
        get_flow += flow.at< Point2f>(frame2.rows / 2, frame2.cols / 2 );
        next_x = frame2.cols / 2 + get_flow.x;
        next_y = frame2.rows / 2 + get_flow.y;
        update_point = Point(next_x, next_y);

        darwRectangle();
        
        //circle(frame2, update_point, 3, CV_RGB(255, 0, 0), -1);
        imshow("frame2", frame2);
        keyboard = waitKey(100);
        if (keyboard == 'q' || keyboard == 27)
            break;
        prvs = next;
    }
}

void DenseOptFlow::darwRectangle() {
    rec_x =  box.x + get_flow.x;
    rec_y = box.y + get_flow.y;
    rec_width = box.width;
    rec_height = box.height;
    tracked_rect = Rect2d(rec_x, rec_y, rec_width, rec_height);
    box = tracked_rect;
}

void DenseOptFlow::drawAllPoints(Mat flow) {
    step = 40;
    for (int y = 0; y < frame2.rows; y += step)
        for (int x = 0; x < frame2.rows; x += step) {
            Point2f flow_point = flow.at< Point2f>(y, x);
            circle(frame2, Point(y, x), 3, CV_RGB(0, 255, 0), -1);
            line(frame2, Point(x, y), Point(flow_point.x + x, flow_point.y + y), CV_RGB(0, 255, 0));
            delta_x += flow_point.x;
            delta_y += flow_point.y;
        }
}

    void DenseOptFlow::showHue(Mat flow){
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
        Point2f test_point = (Point2f) bgr.at<uchar>(70, 70);
        /*circle(bgr, Point(y, x), 5, CV_RGB(0, 255, 0), -1);
        line(bgr, Point(x, y), Point(flow_point.x + x, flow_point.y + y), CV_RGB(0, 255, 0));*/
        cout << "Hue:" << test_point << endl;
        freq = getTickFrequency() / ((double)getTickCount() - timer);
        putText(bgr, to_string(freq), Point(100, 80), FONT_HERSHEY_PLAIN, 1.5, Scalar(0, 0, 255), 2);
        imshow("bgr", bgr);
    }


//void DenseOptFlow::drawROI() {


//}