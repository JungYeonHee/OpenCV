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

	//------------------------ <OSTU�� ����ȭ> -----------------------------------
	Mat blur, th1, th2, th3, th4;

	threshold(src, th1, 127, 255, THRESH_BINARY); // Global Thresholding - �Ӱ谪�� �� �Ȱ��� 
	threshold(src, th2, 0, 255, THRESH_BINARY | THRESH_OTSU); // ������ OSTU�� �Ӱ谪 �ڵ����� ���� 
	
	Size size = Size(5, 5);
	
	GaussianBlur(src, blur, size, 0); // Blurring
	threshold(blur, th3, 0, 255, THRESH_BINARY | THRESH_OTSU); //����þ� ���� ó�� �� OSTU �̿��ؼ� �ڵ����� �Ӱ谪 �� �� 

	imshow("Global Thresholding", th1);
	imshow("Otsu Thresholding", th2);
	imshow("Ostu after Blurring", th3);
	
	//--------------------- <Ʈ���ٷ� �Ӱ谪 ����> ----------------------------
	createTrackbar("�Ӱ谪","result", &threshold_value, max_value, MyThreshold);
	//MyThreshold(0, 0); // �ʱ�ȭ�� ���Ͽ� ȣ��

	//--------------------- <Connected Component Labeling>-----------------------

	// n : ���̺� ���� ��ȯ - ����� ������ �� ������ �м� 
	int n = connectedComponentsWithStats(th3, labels, stats, centroids);

	// �� ���̺��� ���� ����
	vector<Vec3b> colors(n + 1); // �� ���̺��� ������ ����
	colors[0] = Vec3b(0, 0, 0); // colors[0]�� ��� ����, ������
	for (int i = 1; i <= n; i++) { // ���� ������ colors[ ] �迭�� ����
		colors[i] = Vec3b(rand() % 256, rand() % 256, rand() % 256);		
	}

	img_color = cv::Mat::zeros(src.size(), CV_8UC3); //colorful label ���� �����
	for (int y = 0; y < img_color.rows; y++)
		for (int x = 0; x < img_color.cols; x++)
		{
			int label = labels.at<int>(y, x);
			img_color.at<cv::Vec3b>(y, x) = colors[label];

			
		}

	// ���� ���� �ջ� �迭 
	int fiveHundWon[10], oneHundWon[10], tenWon[10], fiftyWon[10];
	int cntFiveHund = 0, cntOneHund = 0, cntTen = 0, cntFifty = 0;

	// stas ��� 
	for (int i = 0; i < n; i++)
	{
		area = stats.at<int>(i, CC_STAT_AREA);
		width = stats.at<int>(i, CC_STAT_WIDTH);
		height = stats.at<int>(i, CC_STAT_HEIGHT);

		cout << i << " : "  << area << " " << width << " " << height << endl;

		// �� width�� ���� ���� ���� �ջ� 
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

	// �� ���� 
	int sum = 10 * cntTen + 50 * cntFifty + 100 * cntOneHund + 500 * cntFiveHund;
	cout << "�� " << sum << " ���Դϴ� !" << endl;

	// �� ���� cnt ��� 
	cout << "5��� " << cntFiveHund << "��" << endl;
	cout << "1��� " << cntOneHund << "��" << endl;
	cout << "5�ʿ� " << cntFifty << "��" << endl;
	cout << "1�ʿ� " << cntTen << "��" << endl;

	// ���Ἲ�� ���̺� ��� ��� 
	cv::imshow("Labeled map", img_color);
	cv::waitKey();

	
	return 0;
}