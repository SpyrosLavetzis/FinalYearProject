#include <opencv2/features2d.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2\tracking.hpp>
#include <iostream>
using namespace std;
using namespace cv;

void featureMatching(Mat img1, Mat img2) {
	/*Mat img1;
	Mat img2;*/
	//this should change to use the frame and roi from MOSSE
	/*img2 = imread("C:\\Users\\spyro\\Desktop\\FrameSmall.jpeg", IMREAD_GRAYSCALE);
	Rect2d box;
	box = selectROI(img2, false, false);
	img1 = img2(box);*/
	
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

	sort(matches.begin(), matches.end());
    matches.resize(2);

	Mat img3;
	drawMatches(img1, img1_keypoints, img2, img2_keypoints, matches, img3);
	//while (waitKey(1) < 1) {
		imshow("image", img3);
	//}
}
