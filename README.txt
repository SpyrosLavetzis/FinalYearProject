This code has been made for my final year project with the aim of creating a real time tracker for small wildlife animals. For the purposes of this project ants were used as the test subjects.

The main code uses an improved version of the Farnerback Dense Optical Flow of OpenCV. To run the code, all that needs to change is the path of the pre-recorded file, or use live feed from a camera.

Code was also developed to evaluate this algorithm and compare it with others from the Main tracking API of OpenCV (MOSSE, KCF and CSRT). This uses the IoU metric as well as precision and recall. One can use the code provided to create a ground truth from any video and create unbiased benchmarks and comparissons.
