Individual Project

Author: Spyridon Lavetzis

Version: 1

Description:


This code has been made for my final year project with the aim of creating a 
real time tracker for small wildlife animals. 
For the purposes of this project ants were used as the test subjects.

The main code uses an improved version of the Farnerback Dense Optical Flow of OpenCV. It can be found in the DenseOptFlow.cpp file.


Code was also developed to evaluate this algorithm and compare it with others 
from the Main tracking API of OpenCV (MOSSE, KCF and CSRT). 
This uses the IoU metric as well as precision and recall. 
One can use the code provided to create a ground truth from any video 
and create unbiased benchmarks and comparissons. This can be found in the GroundTruth.cpp file.

Release Notes:

1) To run the main code (improved Farnerback Dense Optical Flow) with the Ground Truth, one can use the executable file found inside the Debug folder.

2) How to run in Visual Studio:

To run the code, all that needs to change is the path of the pre-recorded file, 
or use live feed from a camera. Use the IndividualProject.vcxproj to load the project. If any issues occur with the OpenCV libraries, they need to be linked again. The version used for this project is 4.4.0.


3) How to run in Windows without Visual Studio:
  a) Load the header and cpp files into a new project.
  b) Link the appropriate OpenCV libraries found the header files.


