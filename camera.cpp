#include "camera.h"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
bool Camera::init()
{
	cam_input = VideoCapture(0);
	if (!cam_input.isOpened())
	{
		printf("Error opening Camera\n");
		return false;
	}
	return true;
}
cv::Mat Camera::shot()
{
	try {


		Mat frame;
	
			cam_input >> frame;

			if (frame.empty())
			{
				printf("Error capture image!\n");

			}
			else
			{
				imshow("Frame", frame);
			
		}
	
		return frame;
	}
	catch (const cv::Exception& ex)
	{
		const char* errMsg = ex.what();
		printf("Cv Exception : {s}", errMsg);

	}
}
void Camera::ContinueShot()
{
	Mat frame;
	int imageCount = 0;
	std::string fileName;
	while (1)
	{
		char key = waitKey(100);
		cam_input >> frame;

		if (frame.empty())
		{
			printf("Error capture image!\n");

		}
		else
		{
			imshow("Frame", frame);
			switch (key) {
			case 'p':
				imageCount++;
				fileName = format("picture%d.jpg", imageCount);
				//fileName = fileName;
				/* 將 frame 寫出成檔案 */
				imwrite(fileName, frame);
				imshow("photo", frame);
				waitKey(500);
				destroyWindow("photo");
				break;
			default:
				break;
			}
		}
	}
	
}