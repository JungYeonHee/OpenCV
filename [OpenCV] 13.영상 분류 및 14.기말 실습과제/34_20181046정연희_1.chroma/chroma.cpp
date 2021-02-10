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

		//���� ���� - ���� ���� 
		morphologyEx(imgThresholded, open_dst, MORPH_OPEN, element);

		//���� ���� - ���� �޿�� 
		morphologyEx(open_dst, close_dst, MORPH_CLOSE, element);

		//�󺧸� 
		Mat labels, stats, centroids;
		int numOfLables = connectedComponentsWithStats(close_dst, labels, stats, centroids, 8, CV_32S); // �ȼ� ���ؼ� ���� �ȼ��� �׷�ȭ


		//�󺧸� �� �̹����� ���簢������ �ѷ��α� 
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

		//�󺧸� �ڽ� 
		rectangle(frame, Point(left, top), Point(left + width, top + height), Scalar(0, 0, 255), 2);

		imshow("��ü ����", close_dst);
		imshow("����", frame);
		 
		//ũ�θ�Ű �ռ� 
		
		Mat dog = imread("dog.jpeg", IMREAD_COLOR);
		resize(dog, dog, Size(width,height));
		
		Mat converted;
		cvtColor(frame, converted, COLOR_BGR2HSV);
		Mat blueScreen = converted.clone();
		
		inRange(converted, Scalar(120-10, 100, 100), Scalar(120+10, 255, 255), blueScreen);

		Mat dst, dst1, inverted;

		Mat roi = frame(Rect(Point(left, top), Point(left + width, top + height)));
		dog.copyTo(roi);
		
		imshow("�ٲ� ����", frame);

		if (waitKey(30) >= 0)
			break;
	}
	return 0;
}