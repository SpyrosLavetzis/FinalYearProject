#include <opencv2/features2d.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2\tracking.hpp>
#include <iostream>
using namespace std;
using namespace cv;

int main() {
	Mat img1;
	Mat img2;
	//Mat img;
	//img1 = imread("C:\\Users\\spyro\\Desktop\\CropAnt.jpeg", IMREAD_GRAYSCALE);
	img2 = imread("C:\\Users\\spyro\\Desktop\\FrameSmall.jpeg", IMREAD_GRAYSCALE);
	Rect2d box;
	//Rect2d box1;
	box = selectROI(img2, false, false);
	img1 = img2(box);
	//box1 = selectROI(img2, false, false);
	//img1 = img2(box2);
	/*while (waitKey(1) < 1) {
		imshow("img", img1);
	}*/
	
	
	Ptr<ORB> orb;
	orb = ORB::create();
	vector<KeyPoint> img1_keypoints, img2_keypoints;
	Mat img1_descriptor, img2_descriptor;

	orb->detectAndCompute(img1, noArray(), img1_keypoints, img1_descriptor);
	orb->detectAndCompute(img2, noArray(), img2_keypoints, img2_descriptor);

	Ptr<BFMatcher> bf;
	bf = BFMatcher::create(NORM_HAMMING, true);
	vector<DMatch> matches;
	bf->match(img1_descriptor, img2_descriptor, matches);
	//vector<DMatch> init_matches;
	//init_matches.insert(matches.begin(), matches.begin() + 10);
	/*for (int i = 0; i < 5; i++) {
		init_matches.at(i) = matches.at(i);
	}*/
	//sort(matches.begin(), matches.end());
	Mat img3;
	matches.resize(5);
	//cout << "Size:"<<matches.size() << endl;
	drawMatches(img1, img1_keypoints, img2, img2_keypoints, matches, img3);
	while (waitKey(1) < 1) {
		imshow("image", img3);
	}
	

	return 0;
}
