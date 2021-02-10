#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


int main(int argc, char** argv)
{
    // 원본 & 히스토그램 출력 
	Mat src = imread("MyPhoto.jpg", IMREAD_COLOR);  // 영상 로드
    if (src.empty())	return -1;

	resize(src, src, Size(480, 480));

    vector<Mat> bgr_planes;	// 영상(Mat)들의 벡터 bgr_planes[0], [1], [2]
    split(src, bgr_planes);	// 입력 영상을 색상별로 분리한다. 

    int histSize = 256;	// 히스토그램 막대 그래프의 개수
    float range[] = { 0, 256 }; 	// 화소값 범위
    const float* histRange = { range };
    bool uniform = true, accumulate = false;

	// 영상에서 각 화소 값의 출현 개수를 구한다.
	Mat b_hist, g_hist, r_hist;
	calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

	// 막대 그래프가 그려지는 영상을 생성한다. 
	int hist_w = 512, hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize); // 상자의 폭 512/256=2
	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0)); // black

	// 값들이 영상을 벗어나지 않도록 정규화한다. 
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	// 히스토그램의 값을 막대로 그린다. 
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

	imshow("(1) 내 칼라 사진", src);
	imshow("(1) 내 칼라 사진 히스토그램", histImage);

	// 원본(흑백) & Histogram Equalization 

	Mat src2 = imread("MyPhoto2.jpg", IMREAD_GRAYSCALE);
	if (src2.empty()) { return -1; }

	Mat dark;
	dark = src2 - 100;

	Mat dst;
	equalizeHist(dark, dst);

	resize(dark, dark, Size(480, 480));
	resize(dst, dst, Size(480, 480));

	imshow("(2) 내 흑백 사진 어둡게", dark);
	imshow("(2) 내 흑백 사진 equalization ", dst);

	
	



	Mat src3;

	src3 = imread("MyPhoto3.jpg", IMREAD_GRAYSCALE);
	
	resize(src3, src3, Size(480, 480));
	imshow("(3) 내 흑백 사진", src3);
	if (!src3.data) { return -1; }

	Mat threshold_image;
	threshold(src3, threshold_image, 100, 255, THRESH_BINARY);

	
	resize(threshold_image, threshold_image, Size(480, 480));

	imshow("(3) 내 흑백 사진 전경, 배경 분리", threshold_image);


	waitKey();
	return 0;
}
