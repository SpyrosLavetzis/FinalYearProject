#include <iostream>
#include <stdio.h>
#include <opencv2\videoio.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2\core.hpp>
#include <opencv2\tracking.hpp>
#include "Tracking.h"

using namespace cv;
using namespace std;

int main() {
	
	Tracking track;
	track.TrackingRun();

	return 0;
}