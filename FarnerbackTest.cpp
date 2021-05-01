#ifdef TEST
#include"DenseOptFlow.h"

#ifdef COMP_RECT
ifstream ifile("C:\\Users\\spyro\\Desktop\\Ants\\ZoomedSlow.txt");
float metric = 0;
long long metric_counter = 0;
#endif // COMP_RECT

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
    //cout << frame.at<int>(0, 0) << endl;
    box = selectROI("test", frame, false, false);
    frame(box).copyTo(frame1);
    cvtColor(frame1, prvs, COLOR_BGR2GRAY);
    next_x = 0;
    next_y = 0;
    threshold = 1.5;
    save_counter = 0;
    sparse_counter = 0;
    frame_counter = 0;
    og_counter = 0;
    hue_counter = 0;
    correction_factor_x = 3;
    correction_factor_y = 3;
    tracked_rect = box;
}

void DenseOptFlow::trackDenseFlow()
{

    while (true) {
        save_counter += 1;
        delta_x = 0;
        delta_y = 0;
        if (break_flag == 1) {
            break;
        }
        //frame_counter += 1;
        //needs to be reset every time
        Mat next;
        capture.read(box_frame);
        timer = (double)getTickCount();
        if (box_frame.empty())
            break;
        box_frame(box).copyTo(frame2);

        cvtColor(frame2, next, COLOR_BGR2GRAY);
        Mat flow(prvs.size(), CV_32FC2);
        calcOpticalFlowFarneback(prvs, next, flow, 0.5, 5, 20, 5, 7, 1.5, 0);
        // visualization

        /*//cout << "Flow:" << flow_point << endl;
        print_flow = flow.at< Point2f>(frame2.rows / 2, frame2.cols / 2);
        get_flow += flow.at< Point2f>(frame2.rows / 2, frame2.cols / 2 );
        next_x = frame2.cols / 2 + get_flow.x;
        next_y = frame2.rows / 2 + get_flow.y;
        next_x = frame2.cols / 2 + get_flow.x;
        next_y = frame2.rows / 2 + get_flow.y;
        update_point = Point(next_x, next_y);
        //cout << "update flow" << update_point << endl;
        /*cout << "get flow" << get_flow << endl;
        circle(frame2, update_point, 3, CV_RGB(0, 255, 0), -1);*/


        /*if (frame_counter == 4) {

            frame_counter = 0;
            darwRectangle();
            get_flow.x = 0;
            get_flow.y = 0;
            //circle(frame2, update_point, 3, CV_RGB(0, 255, 0), -1);
        }*/

        averageFlowTotal(flow);
        #ifdef COMP_RECT
        int truth_x, truth_y;
        ifile >> truth_x >> truth_y;
        Rect2d truth_rect = Rect2d((truth_x - rec_width / 2), (truth_y - rec_height / 2), rec_width, rec_height);
        metric += compareRect(tracked_rect, truth_rect);
        //cout << "metric:" << metric << endl;  
        rectangle(box_frame, truth_rect, Scalar(255, 0, 0), 5, 1);
        rectangle(box_frame, tracked_rect, Scalar(0, 255, 0), 5, 1);
        metric_counter += 1;
        #endif // COMP_RECT


        /*drawAllPoints(flow);
        if (draw_flag == 1) {
            imwrite("C:\\Users\\spyro\\Desktop\\Ants\\OptFlow.png",frame2);
            break;
        }*/

        //drawAllPoints(flow);
        /*if (draw_flag == 1) {
            cout << "HERE" << endl;
            break;
        }*/

        //circle(frame2, update_point, 3, CV_RGB(255, 0, 0), -1);
        saveToFile();
        /*if (save_counter == 10) {
            showHue(flow);
            save_counter = 0;
        }*/
        freq = getTickFrequency() / ((double)getTickCount() - timer);
        cout << "fps" << freq << endl;
        //putText(frame, to_string(freq), Point(100, 80), FONT_HERSHEY_PLAIN, 1.5, Scalar(0, 0, 255), 2);
        #ifndef COMP_RECT
        imshow("frame2", frame2);
        #endif // !COMP_RECT
        #ifdef COMP_RECT
        imshow("frame2", box_frame);
        #endif // COMP_RECT


        keyboard = waitKey(1);
        if (keyboard == 27)
            break;
        prvs = next;

    }
    metric_counter = metric_counter - 4;
    metric = metric / metric_counter;
    //cout << "metric_counter: " << metric_counter << endl;
    cout << "metric: " << (int)(metric * 100) << "%" << endl;

}

void DenseOptFlow::saveToFile() {
    //rectangle(box_frame, box, Scalar(255, 0, 0), 2, 8);
    if (save_counter == 10) {
        name_sparse = "C:\\Users\\spyro\\Desktop\\Ants\\SlowCompare\\DenseMetric" + to_string(sparse_counter) + ".png";
        imwrite(name_sparse, box_frame);
        sparse_counter += 1;
        save_counter = 0;
    }
}

void DenseOptFlow::averageFlowTotal(Mat flow) {
    float sum_delta_x = 0;
    float sum_delta_y = 0;
    x_counter = 0;
    y_counter = 0;
    cout << "farme counter: " << frame_counter << endl;
    for (int y = 0; y < frame2.rows; y++) {
        for (int x = 0; x < frame2.cols; x++) {
            flow_point = flow.at< Point2f>(y, x);

            if (abs(flow_point.y) < threshold) {
                flow_point.y = 0;
                y_counter -= 1;
            }
            if (abs(flow_point.x) < threshold) {
                flow_point.x = 0;
                x_counter -= 1;
            }

            x_counter += 1;
            y_counter += 1;
            //total_counter += 1;
            sum_delta_y += flow_point.y;
            sum_delta_x += flow_point.x;
        }
        //sum_delta_y += flow_point.y;
    }

    //cout << "x_counter:" << x_counter << endl;
    /*if (y_counter == 0) {
        y_counter = 1;
    }
    if (x_counter == 0) {
        x_counter = 1;
    }*/
    //total_counter = total_counter - (y_counter + x_counter) / 2;
    //cout << "total counter:" << total_counter << endl;
   /* dt  = getTickFrequency() / ((double)getTickCount() - timer);
    velocity = dt * sum_delta_y;
    cout << "velocity" << velocity << endl;*/
    /*cout << "sum_x=" << sum_delta_x << endl;
    cout << "sum_y=" << sum_delta_y << endl;*/
    /*if (sum_delta_x > 1000) {
        correction_factor_x = 3;
    }
    else {
        correction_factor_x = 2;
    }
    if (sum_delta_y > 1000) {
        correction_factor_y = 3;
    }
    else {
        correction_factor_y = 2;
    }*/
    /*if (x_counter < 100) {
        x_counter = 0;
    }
    if (y_counter < 100) {
        y_counter = 0;
    }*/

    if (x_counter != 0 && y_counter != 0) {
        sum_delta_x = correction_factor_x * (sum_delta_x / x_counter);
        sum_delta_y = correction_factor_y * (sum_delta_y / y_counter);
    }
    else if (x_counter != 0) {
        sum_delta_x = correction_factor_x * (sum_delta_x / (x_counter));
        sum_delta_y = 0;
    }
    else if (y_counter != 0) {
        sum_delta_y = correction_factor_y * (sum_delta_y / (y_counter));
        sum_delta_x = 0;
    }
    else {
        sum_delta_x = 0;
        sum_delta_y = 0;
    }

    sum_delta_x_t += sum_delta_x;
    sum_delta_y_t += sum_delta_y;
    if (frame_counter == 0) {
        sum_delta_x_t = 0;
        sum_delta_y_t = 0;
        cout << "Please work" << endl;
    }
    cout << "sum x: " << sum_delta_x << endl;
    cout << "sum y: " << sum_delta_y << endl;
    frame_counter += 1;
    if (frame_counter == 4) {
        cout << "At 4" << endl;
        cout << "x:" << sum_delta_x_t << endl;
        cout << "y:" << sum_delta_y_t << endl;
        if (abs(sum_delta_x_t) > 2 /*& abs(sum_delta_x) < 15*/) {
            correction_factor_x = 1.7;
        }
        else {
            correction_factor_x = 1.7;
        }
        if (abs(sum_delta_y_t) > 2 /*& abs(sum_delta_y) < 15*/) {
            correction_factor_y = 1.7;
        }
        else {
            correction_factor_y = 1.7;
        }
        frame_counter = 0;
        darwRectangle();
        sum_delta_x_t = 0;
        sum_delta_y_t = 0;
    }

}

#if 0
void DenseOptFlow::averageFlow(Mat flow) {
    sum_delta_x = 0;
    sum_delta_y = 0;
    x_counter = 0;
    y_counter = 0;
    total_counter = 0;
    for (int y = (frame2.rows / 2 - 10); y < (frame2.rows / 2 + 10); y++) {
        for (int x = (frame2.cols / 2 - 10); x < (frame2.cols / 2 + 10); x++) {
            flow_point = flow.at< Point2f>(y, x);
            /*if (abs(flow_point.y) < threshold) {
                flow_point.y = 0;
                y_counter += 1;
            }
            if (abs(flow_point.x) < threshold) {
                flow_point.x = 0;
                x_counter += 1;
            }*/
            total_counter += 1;
            sum_delta_y += flow_point.y;
            sum_delta_x += flow_point.x;
        }
        //sum_delta_y += flow_point.y;
    }
    //cout << "x:" << sum_delta_x <<  "y:" << sum_delta_y << endl;
    //have a total x and y and if its 0 set whole thing to 0
    /*int denum_x = (frame2.rows * frame2.cols - x_counter);
    int denum_y = (frame2.rows * frame2.cols - y_counter);
    if (denum_x == 0) {
        denum_x = 1;
    }
    if (denum_y == 0) {
        denum_y = 1;
    }*/
    /*cout << "x:" << sum_delta_x << endl;
    cout << "y:" << sum_delta_y << endl;
    cout << "total:" << total_counter << endl;*/
    sum_delta_x = (sum_delta_x / (total_counter / 4));
    sum_delta_y = (sum_delta_y / (total_counter / 4));
    sum_delta_x += sum_delta_x;
    sum_delta_y += sum_delta_y;
    //rec_corner += Point(sum_delta_x, sum_delta_y);
    //cout << rec_corner << endl;
    //rec_corner = Point(sum_delta_x, sum_delta_y);
    /*cout << "denumx:" << denum_x << endl;
    cout << "denumy:" << denum_y << endl;*/

    //cout << "sum" << rec_corner << endl;
    if (frame_counter == 4) {
        cout << "x:" << sum_delta_x << endl;
        cout << "y:" << sum_delta_y << endl;
        frame_counter = 0;
        darwRectangle();
        sum_delta_x = 0;
        sum_delta_y = 0;
    }

}

#endif

void DenseOptFlow::darwRectangle() {
    //cout << "box" << box.x << endl;
    /*rec_x =  box.x + get_flow.x;
    rec_y = box.y + get_flow.y;*/
    rec_x = box.x + sum_delta_x_t;
    rec_y = box.y + sum_delta_y_t;
    /*rec_x = rec_corner.x + box.x;
    rec_y = rec_corner.y + box.y;*/
    //rec_y = box.y - 1;
    /*rec_x = print_flow.x + box.x;
    rec_y = print_flow.y + box.y;*/
    /*rec_x = get_flow.x + box.x;
    rec_y = get_flow.y + box.y;*/
    rec_width = box.width;
    rec_height = box.height;
    tracked_rect = Rect2d(rec_x, rec_y, rec_width, rec_height);
    box = tracked_rect;
}

void DenseOptFlow::drawAllPoints(Mat flow) {
    step = 20;
    for (int y = 0; y < frame2.rows; y += step) {
        for (int x = 0; x < frame2.cols; x += step) {
            draw_flow_point += flow.at< Point2f>(y, x);
            /*delta_x += flow_point.x;
            delta_y += flow_point.y;*/
            //if (frame_counter == 2) {
            circle(frame2, Point(y, x), 3, CV_RGB(0, 255, 0), -1);
            float_x = draw_flow_point.x / 4;
            float_y = draw_flow_point.y / 4;
            /*if (abs(float_x) < 0.5) {
                float_x = 0;
            }
            if (abs(float_y) < 0.5) {
                float_y = 0;
            }*/
            line(frame2, Point(x, y), Point(float_x + x, float_y + y), CV_RGB(0, 255, 0));
            draw_flag = 1;
            //draw_flow_point.x = 0;
            //draw_flow_point.y = 0;
        //}



        }
    }

}

void DenseOptFlow::showHue(Mat flow) {
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
    //Point2f test_point = (Point2f) bgr.at<uchar>(70, 70);
    /*circle(bgr, Point(y, x), 5, CV_RGB(0, 255, 0), -1);
    line(bgr, Point(x, y), Point(flow_point.x + x, flow_point.y + y), CV_RGB(0, 255, 0));*/
    //cout << "Hue:" << test_point << endl;
   /* freq = getTickFrequency() / ((double)getTickCount() - timer);
    putText(bgr, to_string(freq), Point(100, 80), FONT_HERSHEY_PLAIN, 1.5, Scalar(0, 0, 255), 2);*/
    //name_og = "C:\\Users\\spyro\\Desktop\\Ants\\Original" + to_string(og_counter) + ".png";
    name_hue = "C:\\Users\\spyro\\Desktop\\Ants\\SlowCompare\\HueDense" + to_string(hue_counter) + ".png";
    //imwrite(name_og, frame2);
    imwrite(name_hue, bgr);
    //og_counter += 1;
    hue_counter += 1;

}


//void DenseOptFlow::drawROI() {


//}
#endif 