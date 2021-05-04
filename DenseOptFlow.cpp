//Code modified and developed by Spyros Lavetzis. Original code of OpenCV can be found in: https://docs.opencv.org/3.4/d4/dee/tutorial_optical_flow.html

#ifndef RUN_GT
#include"DenseOptFlow.h"

//used for IoU metric
#ifdef COMP_RECT
    ifstream ifile("C:\\Users\\spyro\\Desktop\\Ants\\ZoomedSlow.txt");
    float metric = 0;
    long long metric_counter = 0;
#endif // COMP_RECT

DenseOptFlow::DenseOptFlow()
{
    //pre-recorded video
    capture.open("C:\\Users\\spyro\\Desktop\\Ants\\ZoomedSlow.mp4");
    break_flag = 0;
    //live feed
    //capture.open(0);
    if (!capture.isOpened()) {
        //error in opening the video input
        cerr << "Unable to open file!" << endl;
        break_flag = 1;
    }
    //read first frame
    capture.read(frame);
    //select ROI
    box = selectROI("test", frame, false, false);
    frame(box).copyTo(frame1);
    cvtColor(frame1, prvs, COLOR_BGR2GRAY);
    threshold = 1.5;
    //used by main code to count frames
    frame_counter = 0;
    //correction factor
    correction_factor_x = 1.7;
    correction_factor_y = 1.7;
    tracked_rect = box;
    //counters for saving frames locally
    save_counter = 0;
    sparse_counter = 0;
    og_counter = 0;
    hue_counter = 0;
}

void DenseOptFlow::trackDenseFlow()
{
    while (true) {
        save_counter += 1;
        //check if first frame empty
        if (break_flag == 1) {
            break;
        }
        //needs to be reset every time
        Mat next;
        //read frame
        capture.read(box_frame);
        //for fps calculation
        timer = (double)getTickCount();
        if (box_frame.empty())
            break;
        //frame2 used to showcase ROI as sliding window
        box_frame(box).copyTo(frame2);
        cvtColor(frame2, next, COLOR_BGR2GRAY);
        Mat flow(prvs.size(), CV_32FC2);
        //calculate Farenrback Dense Optical Flow
        calcOpticalFlowFarneback(prvs, next, flow, 0.5, 5, 20, 5, 7, 1.5, 0);
        //apply improvements to result of Optical Flow
        averageFlowTotal(flow);
        //used to calculate metrics
        #ifdef COMP_RECT
            int truth_x, truth_y;
            ifile >> truth_x >> truth_y;
            Rect2d truth_rect = Rect2d((truth_x - rec_width / 2), (truth_y - rec_height / 2), rec_width, rec_height);
            metric += compareRect(tracked_rect, truth_rect);
            rectangle(box_frame, truth_rect, Scalar(255, 0, 0), 5, 1);
            rectangle(box_frame, tracked_rect, Scalar(0, 255, 0), 5, 1);
            metric_counter += 1;
        #endif // COMP_RECT
        //calculate fps
        freq = getTickFrequency() / ((double)getTickCount() - timer);
        cout << "fps" << freq << endl;
        //decide between showing sliding window or rectangle for ROI
        #ifndef COMP_RECT
            imshow("frame2", frame2);
        #endif // !COMP_RECT
        #ifdef COMP_RECT
            imshow("frame2", box_frame);
        #endif // COMP_RECT
        //wait 1ms to show frame
        keyboard = waitKey(1);
        if (keyboard == 27)
            break;
        prvs = next;
        
    }
    //print metric values
    metric_counter = metric_counter - 4;
    metric = metric / metric_counter;
    cout << "metric: " << (int)(metric*100) << "%" << endl;
}

//save frames with metric running
void DenseOptFlow::saveToFile() {
    if (save_counter == 10) {
        name_sparse = "C:\\Users\\spyro\\Desktop\\Ants\\SlowCompare\\DenseMetric" + to_string(sparse_counter) + ".png";
        imwrite(name_sparse, box_frame);
        sparse_counter += 1;
        save_counter = 0;
    }
}

//apply improvements to Optical Flow output
void DenseOptFlow::averageFlowTotal(Mat flow) {
    float sum_delta_x = 0;
    float sum_delta_y = 0;
    x_counter = 0;
    y_counter = 0;
    //get x and y displacement of each pixel
    for (int y = 0; y < frame2.rows; y++) {
        for (int x = 0; x < frame2.cols; x++) {
            flow_point = flow.at< Point2f>(y, x);
            //apply thershold
            if (abs(flow_point.y) < threshold) {
                flow_point.y = 0;
                y_counter -= 1;
            }
            if (abs(flow_point.x) < threshold) {
                flow_point.x = 0;
                x_counter -= 1;
            }
            //calculate total number
            x_counter += 1;
            y_counter += 1;
            //total displacement of each frame
            sum_delta_y += flow_point.y;
            sum_delta_x += flow_point.x;
        }
    }
    //ensure there is no division with 0, apply correction factor and divide by counter
    if (x_counter != 0 && y_counter != 0) {
        sum_delta_x = correction_factor_x*(sum_delta_x / x_counter);
        sum_delta_y = correction_factor_y*(sum_delta_y / y_counter);
    }
    else if (x_counter != 0) {
        sum_delta_x = correction_factor_x *(sum_delta_x / (x_counter));
        sum_delta_y = 0;
    }
    else if (y_counter != 0) {
        sum_delta_y = correction_factor_y *(sum_delta_y / (y_counter));
        sum_delta_x = 0;
    }
    else {
        sum_delta_x = 0;
        sum_delta_y = 0;
    }
    //sum displacement over 4 frames
    sum_delta_x_t += sum_delta_x;
    sum_delta_y_t += sum_delta_y;
    if (frame_counter == 0) {
        sum_delta_x_t = 0;
        sum_delta_y_t = 0;
    }
    frame_counter += 1;
    if (frame_counter == 4) {
        //move ROI every 4 frames
        frame_counter = 0;
        darwRectangle();
        sum_delta_x_t = 0;
        sum_delta_y_t = 0;
    }
}

//move ROI according to total displacement
void DenseOptFlow::darwRectangle() {
    rec_x = box.x + sum_delta_x_t;
    rec_y = box.y + sum_delta_y_t;
    rec_width = box.width;
    rec_height = box.height;
    tracked_rect = Rect2d(rec_x, rec_y, rec_width, rec_height);
    box = tracked_rect;
}

//function to draw Dense Optical Flow
void DenseOptFlow::drawAllPoints(Mat flow) {
    step = 20;
    for (int y = 0; y < frame2.rows; y += step) {
        for (int x = 0; x < frame2.cols; x += step) {
            draw_flow_point += flow.at< Point2f>(y, x);
                circle(frame2, Point(y, x), 3, CV_RGB(0, 255, 0), -1);
                float_x = draw_flow_point.x/4;
                float_y = draw_flow_point.y/4;
                line(frame2, Point(x, y), Point(float_x + x, float_y + y), CV_RGB(0, 255, 0));
                draw_flag = 1;
        }
    }
}

//show Dense Optical Flow results using HSV
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
    //name_og = "C:\\Users\\spyro\\Desktop\\Ants\\Original" + to_string(og_counter) + ".png";
    name_hue = "C:\\Users\\spyro\\Desktop\\Ants\\SlowCompare\\HueDense" + to_string(hue_counter) + ".png";
    //imwrite(name_og, frame2);
    imwrite(name_hue, bgr);
    //og_counter += 1;
    hue_counter += 1;
}
#endif

