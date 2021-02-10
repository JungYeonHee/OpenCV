#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int red, green, blue;
int drawing = false;

void on_trackbar(int, void*) {     }

//4. 이미지에 원 그리기 마우스 콜백함수 
void onMouse(int event, int x, int y, int flags, void* param)
{
	Mat& img = *(Mat*)(param);
	if (event == EVENT_RBUTTONDOWN) {
		circle(img, Point(x, y), 50, Scalar(0, 255, 255), 10);
		putText(img, "I found a dog!", Point(x, y + 200), FONT_HERSHEY_PLAIN, 2.0, 255, 2);
		imshow("src", img);	// 영상이 변경되면 다시 표시한다.
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
	if (src.empty()) { cout << "영상을 읽을 수 없음" << endl; }
	imshow("src", src);

	// 이미지에 원 그리기 
	setMouseCallback("src", onMouse, &src);
	
	// color image to 그레이스케일
	Mat gray, edge, output;
	cvtColor(src, gray, cv::COLOR_BGRA2GRAY);

	imshow("gray", gray);
	imwrite("gray.jpg", gray);
	
	// 도형 그리기 
	line(src, Point(100, 100), Point(300, 300), Scalar(0, 0, 255), 7);
	rectangle(src, Point(250, 30), Point(450, 200), Scalar(0, 255, 0), 5);
	circle(src, Point(100, 300), 60, Scalar(255, 0, 0), 3);
	ellipse(src, Point(300, 350), Point(100, 60), 45, 130, 270, Scalar(255, 255, 255), 5);

	//트랙바 
	namedWindow("src", 1);

	createTrackbar("R", "src", &red, 255, on_trackbar);
	createTrackbar("G", "src", &green, 255, on_trackbar);
	createTrackbar("B", "src", &blue, 255, on_trackbar);


	// 키보드 입력으로 영상의 밝기 조절 
	while (1) {
		int key = waitKeyEx();	// 사용자로부터 키를 기다린다. 
		cout << key << " ";
		if (key == 'q') break;	// 사용자가 ‘q'를 누르면 종료한다. 
		else if (key == 2424832) {	// 키보드 <-
			src -= 50;	// 영상이 어두워진다.
		}
		else if (key == 2555904) {	// 키보드 ->
			src += 50;	// 영상이 밝아진다. 
		}
		imshow("src", src);	// 영상이 변경되었으므로 다시 표시한다.
	}
	waitKey(0);
	return 0;

}

