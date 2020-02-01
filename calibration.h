#include<opencv2/opencv.hpp>
#include"camera.h"
#include <iostream>
#pragma comment(lib, "opecv2416/x64/vc15/lib/opencv_calib3d2413d.lib")
using namespace cv;
enum SideSystem { CALI_LASER, CALI_PROJECTOR, CALI_CAMERA };

class Calibrater
{
public:
	Calibrater() {}
	~Calibrater() {}
private:
	vector<string> m_filenames;
	Size m_borderSize;
	vector<vector<Point2f> > m_srcPoints;
	vector<vector<Point3f> > m_dstPoints;
	double distortionCali = 1;
	Mat cameraMatrix, distCoeffs, map1, map2;
	vector<Mat> rvecs, tvecs;
	
public:

	Calibrater(SideSystem ss);

	void setFilename() {
		m_filenames.clear();

		for (int i = 1; i < 14; i++)
		{
			std::string filename = format("picture%d.jpg", i);
			m_filenames.push_back(filename);
		}

	}

	void setBorderSize(const Size& borderSize) {
		m_borderSize = borderSize;
	}

	void addChessboardPoints() {
		vector<Point2f> srcCandidateCorners;
		vector<Point3f> dstCandidateCorners;
		for (int i = 0; i < m_borderSize.height; i++) {
			for (int j = 0; j < m_borderSize.width; j++) {
				dstCandidateCorners.push_back(Point3f(i, j, 0.0f));
			}
		}

		for (int i = 0; i < m_filenames.size(); i++) {
			Mat image = imread(m_filenames[i], CV_LOAD_IMAGE_GRAYSCALE);
			bool ret = findChessboardCorners(image, m_borderSize, srcCandidateCorners);
			TermCriteria param(TermCriteria::MAX_ITER + TermCriteria::EPS, 30, 0.1);
			cornerSubPix(image, srcCandidateCorners, Size(5, 5), Size(-1, -1), param);
			if (srcCandidateCorners.size() == m_borderSize.area()) {
				addPoints(srcCandidateCorners, dstCandidateCorners);
				cv::drawChessboardCorners(image, m_borderSize, srcCandidateCorners, ret);
				imshow("Chess", image);
			}
		}
	}

	void addPoints(const vector<Point2f>& srcCorners, const vector<Point3f>& dstCorners) {
		m_srcPoints.push_back(srcCorners);
		m_dstPoints.push_back(dstCorners);
	}

	void calibrate(const Mat& src, Mat& dst) {
		Size imageSize = src.size();

		double result = calibrateCamera(m_dstPoints, m_srcPoints, imageSize, cameraMatrix, distCoeffs, rvecs, tvecs);
		printf("calibrate : %d\n", result);
		initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(), Mat(), imageSize, CV_32F, map1, map2);
		std::cout << cameraMatrix << endl;

		Mat newMatrix;
		cameraMatrix.convertTo(newMatrix, CV_32F);

		float* ptr = newMatrix.ptr<float>(0);
		std::cout << "new " << ptr[0] << endl;


		FileStorage fs("test.yml", FileStorage::WRITE);
		fs << "intrinsic" << cameraMatrix;
		fs << "distcoeff" << distCoeffs;

		remap(src, dst, map1, map2, INTER_LINEAR);
		distortionCali = result;
	}

	void Thresholding(Mat& src, Mat& grayImag,int threshold)
	{
	
		if (src.channels() > 1)
		{
			cv::cvtColor(src, grayImag, CV_BGR2GRAY);//±m¦âÂà¦Ç¶¥
		}
		for (int i = 0; i < src.rows; i++)
		{
			uchar* ptr = grayImag.ptr<uchar>(i);
			uchar* srcPtr = src.ptr<uchar>(i);
			
			for (int j = 0; j < src.cols; j++)
			{
				if (ptr[j] < threshold)
				{
					ptr[j] = 0;
				}
				else
				{
					if (srcPtr[j * 3 + 2] < threshold)
					{
						ptr[j] = 0;
					}
				}
			}

		}
		
	}
	void Remap(const Mat& src)
	{
		Size imageSize = src.size();

		initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(), Mat(), imageSize, CV_32F, map1, map2);
		remap(src, src, map1, map2, INTER_LINEAR);
	}

	void GetFocalLength(float* fx, float* fy)
	{
		Mat newMatrix;
		cameraMatrix.convertTo(newMatrix, CV_32F);

		float* ptr = newMatrix.ptr<float>(0);
		std::cout << "new " << ptr[0] << endl;
		*fx = ptr[0];
		*fy = ptr[4];
	}

	void GetImageCenter(float* cx, float* cy)
	{
		Mat newMatrix;
		cameraMatrix.convertTo(newMatrix, CV_32F);
		float* ptr = newMatrix.ptr<float>(0);
		std::cout << "new " << ptr[0] << endl;
		*cx = ptr[0];
		*cy = ptr[4];
	}
private:

};