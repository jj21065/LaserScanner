#include "laser.h"
#include <math.h>
#include <time.h>
#include <iostream>
#define PI 3.14159
#pragma comment(lib, "opecv2416/x64/vc15/lib/opencv_core2413d.lib")


float LineLaser::Gaussian_approx(vector<cv::Point2f>& input)
{
	const int dataSize = input.size();
	cv::Mat cvZ = cv::Mat::zeros(cv::Size(1, dataSize), CV_32F);
	cv::Mat cvX = cv::Mat::zeros(cv::Size(3, dataSize), CV_32F); ;


	for (int i = 0; i < dataSize; i++) // initialize the matrix 
	{
		float* ptr = cvX.ptr<float>(i);
		float* ptrz = cvZ.ptr<float>(i);
		ptr[0] = 1;
		ptr[1] = input[i].x;
		ptr[2] = (double)input[i].x * (double)input[i].x;
		ptrz[0] = input[i].y;
	}
	//cout << X << endl;
	cv::Mat B = ((cvX.t() * cvX).inv()) * cvX.t() * cvZ;

	double S = -1 / B.ptr<float>(0)[2];
	double xmax = B.ptr<float>(0)[1] * S / 2;
	double ymax = B.ptr<float>(0)[0] + xmax * xmax / S;

	return xmax;
}
float Gaussian_approx_2(uchar *input, int dataSize)
{
	
	
	cv::Mat cvZ = cv::Mat::zeros(cv::Size(1, dataSize), CV_32F);
	cv::Mat cvX = cv::Mat::zeros(cv::Size(3, dataSize), CV_32F); ;


	for (int i = 0; i < dataSize; i++) // initialize the matrix 
	{
		float* ptr = cvX.ptr<float>(i);
		float* ptrz = cvZ.ptr<float>(i);
		ptr[0] = 1;
		ptr[1] = i;
		ptr[2] = (double)i * i;
		ptrz[0] = input[i];
	}
	//cout << X << endl;
	cv::Mat B = ((cvX.t() * cvX).inv()) * cvX.t() * cvZ;

	double S = -1 / B.ptr<float>(0)[2];
	double xmax = B.ptr<float>(0)[1] * S / 2;
	double ymax = B.ptr<float>(0)[0] + xmax * xmax / S;

	return xmax;
}
///Find the brightest points in input image of each row , maybe need to binarize the input image first
void LineLaser::FindLaserPoint(cv::Mat& inputImg, vector<cv::Point>* outputPoint)
{
	double START, END;
	START = clock();
	/*---要計算的程式效率區域---*/
	
	const int threshold = 10;

	int col = inputImg.cols;
	int row = inputImg.rows;


	for (int j = 0; j < row; j++)
	{
		uchar* ptr = inputImg.ptr<uchar>(j);
		float max_x =Gaussian_approx_2(ptr, col);
		outputPoint->push_back(cv::Point(max_x, j));

	}

	END = clock();
	//.std::cout << (END - START) / CLOCKS_PER_SEC << endl;
}


double LineLaser::GetDistance(cv::Point inputPoint)
{
	const float baseline = 100.0; // camera to laser distance 
	const float a = 698.1;  /// a = focal length * base line 
	const float b = 4.354;  /// offset (by calibration)
	const float c = 0.006912;   /// c = picel Size


	const float laser_angle = 1.486407302;  /// laser 夾度 85度

	const float focal = a / baseline;

	float center_distance = a / (b - c * inputPoint.x);

	float pitch_angle = atan(((inputPoint.y - Height / 2) * c) / (focal));
	float pitch_distance = center_distance / cos(pitch_angle);

	float laser_to_dist_pt = center_distance * tan(PI / 2 - laser_angle);
	float laser_to_current_pt = sqrt(pitch_distance * pitch_distance + laser_to_dist_pt * laser_to_dist_pt);
	float laser_to_center_pt = sqrt(center_distance * center_distance + laser_to_dist_pt * laser_to_dist_pt);

	float real_distance = sqrt((laser_to_dist_pt) * (laser_to_dist_pt)+pitch_distance * pitch_distance);

	return real_distance;
}


void Scan()
{
	//// init camera, laser , motor

	//// loop
	//{

	//	getRotateAngle();

	//	oneShot();
	//	
	//	FindLaserPoint();

	//	for (;;)
	//	{
	//		GetDistance();
	//	}

	//	MotorRotate();


	//}


}