#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int red, green, blue;
int drawing = false;

void on_trackbar(int, void*) {     }

//4. �̹����� �� �׸��� ���콺 �ݹ��Լ� 
void onMouse(int event, int x, int y, int flags, void* param)
{
	Mat& img = *(Mat*)(param);
	if (event == EVENT_RBUTTONDOWN) {
		circle(img, Point(x, y), 50, Scalar(0, 255, 255), 10);
		putText(img, "I found a dog!", Point(x, y + 200), FONT_HERSHEY_PLAIN, 2.0, 255, 2);
		imshow("src", img);	// ������ ����Ǹ� �ٽ� ǥ���Ѵ�.
	}
	else if (event == EVENT_LBUTTONDOWN)
		drawing = true;
	else if (event == EVENT_MOUSEMOVE) {
		if (drawing == true)
			circle(img, Point(x, y), 3, Scalar(blue, green, red), 10);
	}
	else if (event == EVENT_LBUTTONUP)
		drawing = false;

	imshow("src", img);
}

int main()
{
	Mat src = imread("lenna.jpg", IMREAD_COLOR);
	if (src.empty()) { cout << "������ ���� �� ����" << endl; }
	imshow("src", src);

	// �̹����� �� �׸��� 
	setMouseCallback("src", onMouse, &src);
	
	// color image to �׷��̽�����
	Mat gray, edge, output;
	cvtColor(src, gray, cv::COLOR_BGRA2GRAY);

	imshow("gray", gray);
	imwrite("gray.jpg", gray);
	
	// ���� �׸��� 
	line(src, Point(100, 100), Point(300, 300), Scalar(0, 0, 255), 7);
	rectangle(src, Point(250, 30), Point(450, 200), Scalar(0, 255, 0), 5);
	circle(src, Point(100, 300), 60, Scalar(255, 0, 0), 3);
	ellipse(src, Point(300, 350), Point(100, 60), 45, 130, 270, Scalar(255, 255, 255), 5);

	//Ʈ���� 
	namedWindow("src", 1);

	createTrackbar("R", "src", &red, 255, on_trackbar);
	createTrackbar("G", "src", &green, 255, on_trackbar);
	createTrackbar("B", "src", &blue, 255, on_trackbar);


	// Ű���� �Է����� ������ ��� ���� 
	while (1) {
		int key = waitKeyEx();	// ����ڷκ��� Ű�� ��ٸ���. 
		cout << key << " ";
		if (key == 'q') break;	// ����ڰ� ��q'�� ������ �����Ѵ�. 
		else if (key == 2424832) {	// Ű���� <-
			src -= 50;	// ������ ��ο�����.
		}
		else if (key == 2555904) {	// Ű���� ->
			src += 50;	// ������ �������. 
		}
		imshow("src", src);	// ������ ����Ǿ����Ƿ� �ٽ� ǥ���Ѵ�.
	}
	waitKey(0);
	return 0;

}

