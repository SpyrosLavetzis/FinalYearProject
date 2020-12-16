#include <opencv2/features2d.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2\tracking.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void featureMatching(Mat img1, Mat img2, Mat mask) {
	/*Mat img1;
	Mat img2;*/
	//this should change to use the frame and roi from MOSSE
	/*img2 = imread("C:\\Users\\spyro\\Desktop\\FrameSmall.jpeg", IMREAD_GRAYSCALE);
	Rect2d box;
	box = selectROI(img2, false, false);
	img1 = img2(box);*/
	
	Ptr<ORB> orb;
	orb = ORB::create(500, 1.1f, 8, 31, 0, 4, ORB::HARRIS_SCORE, 31, 20);
	vector<KeyPoint> img1_keypoints, img2_keypoints;
	Mat img1_descriptor, img2_descriptor;

	/*orb->detectAndCompute(img1, noArray(), img1_keypoints, img1_descriptor);
	orb->detectAndCompute(img2, noArray(), img2_keypoints, img2_descriptor);*/

	orb->detectAndCompute(img1, mask, img1_keypoints, img1_descriptor);
	orb->detectAndCompute(img2, mask, img2_keypoints, img2_descriptor);

	Ptr<BFMatcher> bf;
	bf = BFMatcher::create(NORM_HAMMING2, true);
	vector<DMatch> matches;
	bf->match(img1_descriptor, img2_descriptor, matches);

	/*sort(matches.begin(), matches.end());
    matches.resize(3);*/
	float min_dist = 1000, max_dist = 0; //for max function later
	for (int i = 0; i < matches.size(); i++) {
		float dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}
	cout << "Min:" << min_dist << "Max:" << max_dist << endl;
	vector<DMatch> good_matches;
	for (int i = 0; i < matches.size(); i++) {
		if (matches[i].distance <= (1.2 * min_dist)) {
			good_matches.push_back(matches[i]);
		}
	}

	Mat img3;
	drawMatches(img1, img1_keypoints, img2, img2_keypoints, good_matches, img3);
	//while (waitKey(1) < 1) {
		imshow("image", img3);
	//}
}
