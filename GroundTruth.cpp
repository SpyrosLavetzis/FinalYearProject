
#include <fstream>
#include <iostream>
#include <vector>
#include <utility>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
using namespace std;
using namespace cv;

#ifdef RUN_GT

struct Feed {
    VideoCapture capture;
    Mat frame;
    vector<pair<int, int>>* v;
};

void mouseClick(int event, int x, int y, int flags, void* userdata) {
    Feed* f = (Feed*)userdata;
    if (event == EVENT_LBUTTONDOWN) {
        f->v->push_back(pair<int, int>(x, y));
        f->capture.read(f->frame);
    }
}

int main() {
    Feed feed;
    vector<pair<int, int>> centers;
    feed.v = &centers;
	feed.capture.open("C:\\Users\\spyro\\Desktop\\Ants\\ZoomedSlow.mp4");
    feed.capture.read(feed.frame);
    namedWindow("Truth", WINDOW_NORMAL);
    setMouseCallback("Truth", mouseClick, &feed);
    if (!feed.capture.isOpened()) {
        //error in opening the video input
        cerr << "Unable to open file!" << endl;
    }
    while (1) {
        if (!feed.frame.empty()) {
            imshow("Truth", feed.frame);
        }
        else
            break;
        int k = waitKey(1);
        if (k == 27)
            break;
    }
    ofstream file("C:\\Users\\spyro\\Desktop\\Ants\\ZoomedSlow.txt");
    for (pair<int, int> point : centers) {
        file << point.first << " " << point.second << endl;
    }
    file.close();
    ifstream ifile("C:\\Users\\spyro\\Desktop\\Ants\\ZoomedSlow.txt");
    int x, y;
    while (ifile >> x >> y) {
        cout << x << " " << y << endl;
    }
	return 0;
}

#endif

float compareRect(Rect2d rect1, Rect2d rect2) {
    Rect2d rect_intersection = rect1 & rect2;
    int area_inter = rect_intersection.area();
    int area_union = rect1.area() + rect2.area() - area_inter;
    return ((float)area_inter) / area_union;
}
