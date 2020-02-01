//#include<opencv2/opencv.hpp>
#include "camera.h"
#include <vector>

using namespace std;

class LineLaser:Camera
{
	public : 
	
	LineLaser(){}
	~LineLaser(){}
	
	bool init();
	bool trigger( bool on_off );
	
	bool isCalibrated();
	
	void readParameters( const char* );
	void wirteParameters( const char* );
	
	
	///Find the brightest point of the inputimg
	void FindLaserPoint(cv::Mat &inputImg, vector<cv::Point>* ouputPoint);

	double GetDistance(cv::Point inputPoint);

	float Gaussian_approx(vector<cv::Point2f> &input);

	//get the 3d coordinate of laser line on the chessboard
	bool calibration(
		std::vector<std::vector<cv::Point3f>> laserCoord,
		std::vector<cv::Mat> rVecs,
		std::vector<cv::Mat> tVecs
	);
	
	private : 
	
	bool isCali;
	
	//the coeff of plane function
	cv::Point3f coeff;
	
};