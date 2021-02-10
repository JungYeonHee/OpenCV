#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


int main(int argc, char** argv)
{
    // ���� & ������׷� ��� 
	Mat src = imread("MyPhoto.jpg", IMREAD_COLOR);  // ���� �ε�
    if (src.empty())	return -1;

	resize(src, src, Size(480, 480));

    vector<Mat> bgr_planes;	// ����(Mat)���� ���� bgr_planes[0], [1], [2]
    split(src, bgr_planes);	// �Է� ������ ���󺰷� �и��Ѵ�. 

    int histSize = 256;	// ������׷� ���� �׷����� ����
    float range[] = { 0, 256 }; 	// ȭ�Ұ� ����
    const float* histRange = { range };
    bool uniform = true, accumulate = false;

	// ���󿡼� �� ȭ�� ���� ���� ������ ���Ѵ�.
	Mat b_hist, g_hist, r_hist;
	calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

	// ���� �׷����� �׷����� ������ �����Ѵ�. 
	int hist_w = 512, hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize); // ������ �� 512/256=2
	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0)); // black

	// ������ ������ ����� �ʵ��� ����ȭ�Ѵ�. 
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	// ������׷��� ���� ����� �׸���. 
	for (int i = 0; i < 255; i++) {
		line(histImage, Point(bin_w * (i), hist_h),
			Point(bin_w * (i), hist_h - b_hist.at<float>(i)),
			Scalar(255, 0, 0));  // blue
		line(histImage, Point(bin_w * (i), hist_h),
			Point(bin_w * (i), hist_h - g_hist.at<float>(i)),
			Scalar(0, 255, 0));  // green
		line(histImage, Point(bin_w * (i), hist_h),
			Point(bin_w * (i), hist_h - r_hist.at<float>(i)),
			Scalar(0, 0, 255));  // red
	}

	imshow("(1) �� Į�� ����", src);
	imshow("(1) �� Į�� ���� ������׷�", histImage);

	// ����(���) & Histogram Equalization 

	Mat src2 = imread("MyPhoto2.jpg", IMREAD_GRAYSCALE);
	if (src2.empty()) { return -1; }

	Mat dark;
	dark = src2 - 100;

	Mat dst;
	equalizeHist(dark, dst);

	resize(dark, dark, Size(480, 480));
	resize(dst, dst, Size(480, 480));

	imshow("(2) �� ��� ���� ��Ӱ�", dark);
	imshow("(2) �� ��� ���� equalization ", dst);

	
	



	Mat src3;

	src3 = imread("MyPhoto3.jpg", IMREAD_GRAYSCALE);
	
	resize(src3, src3, Size(480, 480));
	imshow("(3) �� ��� ����", src3);
	if (!src3.data) { return -1; }

	Mat threshold_image;
	threshold(src3, threshold_image, 100, 255, THRESH_BINARY);

	
	resize(threshold_image, threshold_image, Size(480, 480));

	imshow("(3) �� ��� ���� ����, ��� �и�", threshold_image);


	waitKey();
	return 0;
}
