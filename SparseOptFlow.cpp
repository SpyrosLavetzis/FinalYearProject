#ifndef RUN_GT

#include "SparseOptFlow.h"
void selectPoint(int event, int x, int y, int flags, void* userdata);
int sparse_counter, save_counter;
String name_sparse;

SparseOptFlow::SparseOptFlow() {
    
    capture.open("C:\\Users\\spyro\\Desktop\\Ants\\ZoomedFast.mp4");
    sparse_counter = 0;
    save_counter = 0;
    //capture.open(0);
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
    // Create a mask image for drawing purposes
    mask = Mat::zeros(old_frame.size(), old_frame.type());
    frame_counter = 0;
    //callback so video can play while no point is selected
    setMouseCallback("Test", selectPoint, this);
}

//selects points for Sparse Optical Flow
void selectPoint(int event, int x, int y, int flags, void *userdata ) {
    Point point;
    //use x and y of mouse
    if (event == EVENT_LBUTTONDOWN) {
        point = Point(x, y);
        SparseOptFlow* obj = (SparseOptFlow*) userdata;
        obj->p0.push_back(point);
        obj->trackOptFlow();
    }
}

//keep playing video until points are selected
void SparseOptFlow::playVideo() {
    for (;;) {
        capture.read(frame);
        imshow("Test", frame);
        //update every 1 ms and break if esc key is pressed
        if (waitKey(1000) == 27) { 
            break;
        }
    }
}

//main algorithm
void SparseOptFlow::trackOptFlow() {
    while (true) {
        save_counter += 1;
        // for fps calculation
        timer = (double)getTickCount();
        capture.read(frame);
        if (frame.empty())
            break;
        frame_counter += 1;
        cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
        //set up Optical Flow
        criteria = TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 30, 0.01);
        // calculate Sparse Optical Flow
        calcOpticalFlowPyrLK(old_gray, frame_gray, p0, p1, status, err, Size(21, 21), 4, criteria);
        //draw tracked points and show trajectory
        for (uint i = 0; i < p0.size(); i++)
        {
            // Select good points
            if (status[i] == 1) {
                good_new.push_back(p1[i]);
                // draw the tracks
                line(mask, p1[i], p0[i], colors[i], 6);
                circle(frame, p1[i], 7, colors[i], -1);
            }
        }
        //add original frame with mask
        add(frame, mask, img);
        //save frames locally
        if (save_counter == 10) {
            name_sparse = "C:\\Users\\spyro\\Desktop\\Ants\\FastCompare\\FastSparse" + to_string(sparse_counter) + ".png";
            imwrite(name_sparse, img);
            sparse_counter += 1;
            save_counter = 0;
        }
        //show fps
        freq = getTickFrequency() / ((double)getTickCount() - timer);
        putText(img, to_string(freq), Point(100, 80), FONT_HERSHEY_PLAIN, 1.5, Scalar(0, 0, 255), 2);
        imshow("Test", img);
        //wait 1ms to show frame
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
    
#endif