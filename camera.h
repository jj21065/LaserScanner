#ifndef CAMERA_H
#define CAMERA_H

#include<opencv2/opencv.hpp>
#include <opencv2\highgui\highgui.hpp>

class Camera
{
public:

	Camera() {}
	~Camera() {}

	bool init();
	bool open();

	bool isCalibrated();

	cv::Mat shot();
	void ContinueShot();
	bool calibration(
		std::vector<cv::Point3f> objectPoints,
		std::vector<cv::Point2f> imagePoints,
		int opencvFlag
	);

	int Height; // image height 
	int Width;  // image width

	//get the copy of rotation and tanslation
	std::vector<cv::Mat> getRotation();
	std::vector<cv::Mat> getTanslation();

	void readParameters(const char*);
	void wirteParameters(const char*);

private:
	cv::VideoCapture cam_input;

	int ID;
	int row;
	int col;

	bool isCali;

	cv::Mat intrix;
	std::vector<cv::Mat> rVecs;
	std::vector<cv::Mat> tVecs;
};

#endif // !1