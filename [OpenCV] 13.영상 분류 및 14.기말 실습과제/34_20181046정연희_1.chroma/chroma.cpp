#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


int main()
{
	Mat open_dst, dst, close_dst;

	VideoCapture cap(0);
	if (!cap.isOpened()) { cout << "file not found" << endl; }


	while (1)
	{
		Mat imgHSV;
		Mat frame;
		cap >> frame;
		cvtColor(frame, imgHSV, COLOR_BGR2HSV);
		
		Mat imgThresholded;
		inRange(imgHSV, Scalar(120-10, 100, 100), Scalar(120+10, 255, 255), imgThresholded);


		Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));

		//열림 연산 - 잡음 제거 
		morphologyEx(imgThresholded, open_dst, MORPH_OPEN, element);

		//닫힘 연산 - 구멍 메우기 
		morphologyEx(open_dst, close_dst, MORPH_CLOSE, element);

		//라벨링 
		Mat labels, stats, centroids;
		int numOfLables = connectedComponentsWithStats(close_dst, labels, stats, centroids, 8, CV_32S); // 픽셀 정해서 인접 픽셀들 그룹화


		//라벨링 된 이미지에 직사각형으로 둘러싸기 
		int max = -1, idx = 0;
		for (int j = 1; j < numOfLables; j++) {
			int area = stats.at<int>(j, CC_STAT_AREA);
			if (max < area)
			{
				max = area;
				idx = j;
			}
		}

		int left = stats.at<int>(idx, CC_STAT_LEFT);
		int top = stats.at<int>(idx, CC_STAT_TOP);
		int width = stats.at<int>(idx, CC_STAT_WIDTH);
		int height = stats.at<int>(idx, CC_STAT_HEIGHT);

		//라벨링 박스 
		rectangle(frame, Point(left, top), Point(left + width, top + height), Scalar(0, 0, 255), 2);

		imshow("객체 추적", close_dst);
		imshow("원본", frame);
		 
		//크로마키 합성 
		
		Mat dog = imread("dog.jpeg", IMREAD_COLOR);
		resize(dog, dog, Size(width,height));
		
		Mat converted;
		cvtColor(frame, converted, COLOR_BGR2HSV);
		Mat blueScreen = converted.clone();
		
		inRange(converted, Scalar(120-10, 100, 100), Scalar(120+10, 255, 255), blueScreen);

		Mat dst, dst1, inverted;

		Mat roi = frame(Rect(Point(left, top), Point(left + width, top + height)));
		dog.copyTo(roi);
		
		imshow("바뀐 원본", frame);

		if (waitKey(30) >= 0)
			break;
	}
	return 0;
}