#include <iostream>
#include "laser.h"
#include "camera.h"
#include "calibration.h"
int main()
{
	LineLaser laser;
	Camera camera;
	Calibrater calibrater;
	camera.init();
	calibrater.setFilename();
	calibrater.setBorderSize(Size(6,4));
	calibrater.addChessboardPoints();
	Mat src = imread("picture1.jpg", 0);
	Mat dst;
	calibrater.calibrate(src, dst);

	while (1)
	{
		src = camera.shot();
		calibrater.Remap(src);
		cv::Mat dst;
		calibrater.Thresholding(src, dst, 210);
		imshow("Undistorted Image", dst);


		vector<cv::Point> output;
		laser.FindLaserPoint(dst, &output);
		std::cout << laser.GetDistance(output[40]) << endl;
	/*	for (int i = 0; i < 30; i++)
		{
			std::cout << output[i] << endl;
		}*/
		cv::waitKey(35);
	}

	//camera.shot();
	/*camera.ContinueShot();
	cv::Mat img = cv::imread("test.png", CV_LOAD_IMAGE_GRAYSCALE);
	cv::imshow("test",img);*/


	cv::waitKey(0);
	system("pause");
	return 0;
}