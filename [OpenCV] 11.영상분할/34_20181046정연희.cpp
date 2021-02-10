#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int threshold_value = 128;
int threshold_type = 0;
const int max_value = 255;
const int max_binary_value = 255;
Mat src, src_gray, dst;
Mat img_edge, labels, centroids, img_color, stats;
int area, width, height;

static void MyThreshold(int, void*)
{
	threshold(src, dst, threshold_value, max_binary_value, threshold_type);
	imshow("result", dst);
}


int main()
{

	src = imread("coins5.jpg", IMREAD_GRAYSCALE);

	if (src.empty()) { return -1; }
	imshow("result", src);

	//------------------------ <OSTU의 이진화> -----------------------------------
	Mat blur, th1, th2, th3, th4;

	threshold(src, th1, 127, 255, THRESH_BINARY); // Global Thresholding - 임계값이 다 똑같음 
	threshold(src, th2, 0, 255, THRESH_BINARY | THRESH_OTSU); // 원본에 OSTU로 임계값 자동으로 설정 
	
	Size size = Size(5, 5);
	
	GaussianBlur(src, blur, size, 0); // Blurring
	threshold(blur, th3, 0, 255, THRESH_BINARY | THRESH_OTSU); //가우시안 블러링 처리 후 OSTU 이용해서 자동으로 임계값 준 것 

	imshow("Global Thresholding", th1);
	imshow("Otsu Thresholding", th2);
	imshow("Ostu after Blurring", th3);
	
	//--------------------- <트랙바로 임계값 조절> ----------------------------
	createTrackbar("임계값","result", &threshold_value, max_value, MyThreshold);
	//MyThreshold(0, 0); // 초기화를 위하여 호출

	//--------------------- <Connected Component Labeling>-----------------------

	// n : 레이블 개수 반환 - 연결된 성분이 몇 개인지 분석 
	int n = connectedComponentsWithStats(th3, labels, stats, centroids);

	// 각 레이블의 색상 설정
	vector<Vec3b> colors(n + 1); // 각 레이블의 색상을 저장
	colors[0] = Vec3b(0, 0, 0); // colors[0]는 배경 색상, 검정색
	for (int i = 1; i <= n; i++) { // 랜덤 색상을 colors[ ] 배열에 저장
		colors[i] = Vec3b(rand() % 256, rand() % 256, rand() % 256);		
	}

	img_color = cv::Mat::zeros(src.size(), CV_8UC3); //colorful label 영상 만들기
	for (int y = 0; y < img_color.rows; y++)
		for (int x = 0; x < img_color.cols; x++)
		{
			int label = labels.at<int>(y, x);
			img_color.at<cv::Vec3b>(y, x) = colors[label];

			
		}

	// 동전 갯수 합산 배열 
	int fiveHundWon[10], oneHundWon[10], tenWon[10], fiftyWon[10];
	int cntFiveHund = 0, cntOneHund = 0, cntTen = 0, cntFifty = 0;

	// stas 출력 
	for (int i = 0; i < n; i++)
	{
		area = stats.at<int>(i, CC_STAT_AREA);
		width = stats.at<int>(i, CC_STAT_WIDTH);
		height = stats.at<int>(i, CC_STAT_HEIGHT);

		cout << i << " : "  << area << " " << width << " " << height << endl;

		// 각 width에 따라 동전 갯수 합산 
		if (width > 50 && width < 60 ) {
			tenWon[cntTen] = width;
			cntTen++;
		}
		else if (width >= 60 && width < 70) {
			fiftyWon[cntFifty] = width;
			cntFifty++;
		}
		else if (width >= 70 && width <=76) {
			oneHundWon[cntOneHund] = width;
			cntOneHund++;
		}
		else if( width >= 77 && width <= 87) {
			fiveHundWon[cntFiveHund] = width;
			cntFiveHund++;
		}
	}

	// 총 가격 
	int sum = 10 * cntTen + 50 * cntFifty + 100 * cntOneHund + 500 * cntFiveHund;
	cout << "총 " << sum << " 원입니다 !" << endl;

	// 각 동전 cnt 출력 
	cout << "5백원 " << cntFiveHund << "개" << endl;
	cout << "1백원 " << cntOneHund << "개" << endl;
	cout << "5십원 " << cntFifty << "개" << endl;
	cout << "1십원 " << cntTen << "개" << endl;

	// 연결성분 레이블링 결과 출력 
	cv::imshow("Labeled map", img_color);
	cv::waitKey();

	
	return 0;
}