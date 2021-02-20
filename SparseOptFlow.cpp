#include "SparseOptFlow.h"
void selectPoint(int event, int x, int y, int flags, void* userdata);
SparseOptFlow::SparseOptFlow() {
    //capture.open("C:\\Users\\spyro\\Desktop\\Ants\\ZoomedSlow.mp4");

    capture.open(0);
    if (!capture.isOpened()) {
        //error in opening the video input
        cout << "Unable to open file!" << endl;
        // break;
    }

    //create random colour using the rgb format
    for (int i = 0; i < 100; i++)
    {
        int r = rng.uniform(0, 256);
        int g = rng.uniform(0, 256);
        int b = rng.uniform(0, 256);
        colors.push_back(Scalar(r, g, b));
    }

    // Take first frame and choose points to track
    capture.read(old_frame);
    cvtColor(old_frame, old_gray, COLOR_BGR2GRAY);
    //create window
    namedWindow("Test", WINDOW_NORMAL);
    resizeWindow("Test", 1000, 700);
    /*for (int i = 0; i < 1; i++) {
        Rect box = selectROI("Test", old_frame, false, false);
        p0.push_back(box.br());
    }*/
    // Create a mask image for drawing purposes
    mask = Mat::zeros(old_frame.size(), old_frame.type());
    frame_counter = 0;
    setMouseCallback("Test", selectPoint, this);
    /*imshow("Test", old_frame);
    waitKey(0);*/
    
}


    
void selectPoint(int event, int x, int y, int flags, void *userdata ) {
    Point point;
    if (event == EVENT_LBUTTONDOWN) {
        point = Point(x, y);
        //vector<Point2f>* p0 = (vector<Point2f>*)userdata;
        SparseOptFlow* obj = (SparseOptFlow*) userdata;
        obj->p0.push_back(point);
        obj->trackOptFlow();
    }

}

void SparseOptFlow::playVideo() {
    for (;;) {
        //timer = (double)getTickCount();
        capture.read(frame);
       // freq = getTickFrequency() / ((double)getTickCount() - timer);
        //putText(frame, to_string(freq), Point(100, 80), FONT_HERSHEY_PLAIN, 1.5, Scalar(0, 0, 255), 2);
        imshow("Test", frame);
        if (waitKey(1) == 27) { //update every 1 ms and break if esc key is pressed
            break;
        }
    }
}

void SparseOptFlow::trackOptFlow() {
    while (true) {
        timer = (double)getTickCount();
        capture.read(frame);
        if (frame.empty())
            break;
        frame_counter += 1;
        cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
        // calculate optical flow

        
        //criteria = TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 10, 0.03);
        criteria = TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 30, 0.01);

        calcOpticalFlowPyrLK(old_gray, frame_gray, p0, p1, status, err, Size(21, 21), 4, criteria);
        //vector<Point2f> good_new;
        //cout << status.size() << endl;
        for (uint i = 0; i < p0.size(); i++)
        {
            // Select good points
            if (status[i] == 1) {
                good_new.push_back(p1[i]);
                // draw the tracks
                line(mask, p1[i], p0[i], colors[i], 2);
                circle(frame, p1[i], 5, colors[i], -1);
            }
        }
        
        add(frame, mask, img);
        
        freq = getTickFrequency() / ((double)getTickCount() - timer);
        putText(img, to_string(freq), Point(100, 80), FONT_HERSHEY_PLAIN, 1.5, Scalar(0, 0, 255), 2);
        imshow("Test", img);
        
        keyboard = waitKey(1);
        if (keyboard == 27)
            break;
        // Now update the previous frame and previous points
        old_gray = frame_gray.clone();
        p0 = good_new;
        //reset vector as we only want to store current and previous position (not the ones before that)
        good_new.clear();
    }
}

void SparseOptFlow::updatePoints() {
    if (frame_counter == 30) {

    }

}



    
