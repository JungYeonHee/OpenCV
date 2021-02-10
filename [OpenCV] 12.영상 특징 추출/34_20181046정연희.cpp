#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// line Detection
int threshold_value = 50;
int minLineLength = 50;
int maxLineGap = 50;

// Circle Detection
int minDist = 8;
int parma1 = 200;
int parma2 = 50;
int minRadius = 0;
int maxRadius = 0;

// Edge Detection
Mat gray_corner;
int thresh = 150, blockSize = 2, apertureSize = 3;
double k = 0.04;
int thresh_thresh = 150;

const int max_value = 255;
const int max_binary_value = 255;
Mat src_line, src_line_color,src_circle, src_corner;

Mat dst, cdst;
vector<Vec4i> lines;

static void MyLineThreshold(int, void*) //���� ���� 
{

	threshold_value = getTrackbarPos("threshold_value", "src_line_color");
	minLineLength = getTrackbarPos("minLineLength", "src_line_color");
	maxLineGap = getTrackbarPos("maxLineGap", "src_line_color");

	HoughLinesP(dst, lines, 1, CV_PI / 180, threshold_value, minLineLength, maxLineGap);

	for (size_t i = 0; i < lines.size(); i++) {
		Vec4i l = lines[i];
		line(src_line_color, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, LINE_AA);
	}

	imshow("src_line_color", src_line_color);
}

vector<Vec3f> circles;
static void MyCircleThreshold(int, void*)
{

	minDist = getTrackbarPos("minDist", "Hough Circle Transform");
	parma1 = getTrackbarPos("parma1", "Hough Circle Transform");
	parma2 = getTrackbarPos("parma2", "Hough Circle Transform");
	minRadius = getTrackbarPos("minRadius", "Hough Circle Transform");
	maxRadius = getTrackbarPos("maxRadius", "Hough Circle Transform");

	Mat gray;
	cvtColor(src_circle, gray, COLOR_BGR2GRAY); // �׷��̽����Ϸ� ��ȯ
	GaussianBlur(gray, gray, Size(9, 9), 2, 2); // ����þ� ���� ����

	vector<Vec3f> circles; // ���� �����ϴ� ���� ��ȯ
	HoughCircles(gray, circles, HOUGH_GRADIENT, 1, gray.rows / minDist, parma1, parma2, minRadius, maxRadius);
	
	for (size_t i = 0; i < circles.size(); i++) { // ���� ���� ���� �׸���.
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		circle(src_circle, center, 3, Scalar(0, 255, 0), -1, 8, 0); // ���� �߽�
		circle(src_circle, center, radius, Scalar(0, 0, 255), 2, 8, 0); // ���� �׸���.
	}
	

	imshow("Hough Circle Transform", src_circle);
}


static void MyEdgeThreshold(int, void*)
{
	thresh_thresh = getTrackbarPos("thresh_thresh", "src_corner");
	
	Mat dst_corner, dst_norm, dst_norm_scaled;
	dst_corner = Mat::zeros(src_corner.size(), CV_32FC1);
	cornerHarris(gray_corner, dst_corner, blockSize, apertureSize, k);
	normalize(dst_corner, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	convertScaleAbs(dst_norm, dst_norm_scaled); //8 bit, absolute value

	for (int j = 0; j < dst_norm.rows; j++) { //draw red circles at the corners
		for (int i = 0; i < dst_norm.cols; i++) {
			if ((int)dst_norm.at<float>(j, i) > thresh_thresh) {
				circle(src_corner, Point(i, j), 5, Scalar(0, 0, 255), 2, 8, 0);
			}
		}
	}
	imshow("src_corner", src_corner);
}


void on_trackbar(int, void*) { }

int main()
{

		// Huff Transofrm ���� ��ȯ - ���� ���� 

		src_line = imread("line.jpg", IMREAD_GRAYSCALE);
		if (src_line.empty()) { cout << "can not open " << endl; return -1; }

		src_line_color = imread("line.jpg", 1);
		if (src_line_color.empty()) {
			cout << "can not open " << endl; return -1;
		}
		imshow("src_line_color", src_line_color);

		Canny(src_line, dst, 100, 200); // edge detection - ���� ��濡 ȭ��Ʈ ��
		cvtColor(dst, cdst, COLOR_GRAY2BGR);
	
		createTrackbar("threshold_value", "src_line_color", &threshold_value, max_value, MyLineThreshold);
		createTrackbar("minLineLength", "src_line_color", &minLineLength, max_value, MyLineThreshold);
		createTrackbar("maxLineGap", "src_line_color", &maxLineGap, max_value, MyLineThreshold);
		
	//--------------------------------------------------------------------------------	
		// Circle Hough Transform ���� ���� ��ȯ - �� ���� 

		src_circle = imread("circle.jpg", 1);
		if (src_circle.empty()) { cout << "can not open " << endl; return -1; }

		imshow("Hough Circle Transform", src_circle);
		
		Mat gray;
		cvtColor(src_circle, gray, COLOR_BGR2GRAY); // �׷��̽����Ϸ� ��ȯ
		GaussianBlur(gray, gray, Size(9, 9), 2, 2); // ����þ� ���� ����
	
		createTrackbar("minDist", "Hough Circle Transform", &minDist, max_value, MyCircleThreshold);
		createTrackbar("parma1", "Hough Circle Transform", &parma1, max_value, MyCircleThreshold);
		createTrackbar("parma2", "Hough Circle Transform", &parma2, max_value, MyCircleThreshold);
		createTrackbar("minRadius", "Hough Circle Transform", &minRadius, max_value, MyCircleThreshold);
		createTrackbar("maxRadius", "Hough Circle Transform", &maxRadius, max_value, MyCircleThreshold);

		// Harris Corner Detection - �ڳ� ����
		
		src_corner = imread("corner.jpg", 1);
		if (src_corner.empty()) { cout << "can not open " << endl; return -1; }

		cvtColor(src_corner, gray_corner, COLOR_BGR2GRAY);
		
		imshow("src_corner", src_corner);
		

		createTrackbar("thresh_thresh", "src_corner", &thresh_thresh, max_value, MyEdgeThreshold);

		waitKey();


		return 0;
}