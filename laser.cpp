#include "laser.h"
#include <math.h>

#define PI 3.14159
#pragma comment(lib, "opecv2416/x64/vc15/lib/opencv_core2413d.lib")


float LineLaser::Gaussian_approx(vector<cv::Point2f> &input)
{
	const int dataSize = input.size();

	MatrixXd Z(dataSize, 1);
	MatrixXd X(dataSize, 3);

	for (int i = 0; i < dataSize; i++) // initialize the matrix 
	{

		X(i, 0) = 1;
		X(i, 1) = input[i].x;
		X(i, 2) = (double)input[i].x * (double)input[i].x;

		Z(i, 0) = input[i].y;
	}
	//cout << X << endl;
	
	MatrixXd B = ((X.transpose() * X).inverse()) * X.transpose() * Z; // Get the coefficient of the gaussian approximation
	double S = -1 / B(2);
	double xmax = B(1) * S / 2;
	double ymax = B(0) + xmax * xmax / S;
	cout << "Max x  = " << xmax << endl;
	cout << "Max y = " << ymax << endl;

	return xmax;
}

///Find the brightest points in input image of each row , maybe need to binarize the input image first
void LineLaser::FindLaserPoint(cv::Mat& inputImg, vector<cv::Point> *outputPoint)
{
	const int threshold =10;
	const int minum_number = 3;
	int raisePos = 0;
	int fallPos = 0;
	int centerPos = 0;
	unsigned char maxPwr = 0;

	int col = inputImg.cols;
	int row = inputImg.rows;


	for (int j = 0; j < row; j++)
	{
	
		vector<cv::Point2f> rowData;
		for (int i = 0; i < col; i++)
		{
			if (inputImg.at<uchar>(j, i) > threshold)
			{
				rowData.push_back(cv::Point(i,inputImg.at<uchar>(j, i)));
			}		
		}
		if (rowData.size() > minum_number)
		{
			float max_x = Gaussian_approx(rowData);
			outputPoint->push_back(cv::Point(max_x, j)); // (x,y)
		}
	
	}
}


double LineLaser::GetDistance(cv::Point inputPoint)
{
	const float baseline = 100.0; // camera to laser distance 
	const float a = 698.1;  /// a = focal length * base line 
	const float b = 4.354;  /// offset (by calibration)
	const float c = 0.006912;   /// c = picel Size


	const float laser_angle = 1.486407302;  /// laser §¨«× 85«×

	const float focal = a / baseline;    

	float center_distance = a / (b - c * inputPoint.x);

	float pitch_angle = atan(((inputPoint.y - Height/ 2) * c) / (focal));
	float pitch_distance = center_distance / cos(pitch_angle);

	float laser_to_dist_pt = center_distance * tan(PI / 2 - laser_angle);
	float laser_to_current_pt = sqrt(pitch_distance * pitch_distance + laser_to_dist_pt * laser_to_dist_pt);
	float laser_to_center_pt = sqrt(center_distance * center_distance + laser_to_dist_pt * laser_to_dist_pt);

	float real_distance = sqrt((laser_to_dist_pt) * (laser_to_dist_pt) + pitch_distance * pitch_distance);

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