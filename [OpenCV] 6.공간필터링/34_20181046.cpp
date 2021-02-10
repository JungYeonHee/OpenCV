#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;



void SobelPixels(Mat src)
{
	Mat grad_x(src.rows, src.cols, CV_16S);
	Mat grad_y(src.rows, src.cols, CV_16S);
	int weightx[] = { 1,0,-1,  2,0,-2, 1,0,-1 };	// sobel mask X
	int weighty[] = { -1,-2,-1,  0,0,0, 1,2,1 }; 	// sobel mask Y

	for (int r = 1; r < src.rows - 1; r++)  // for sobel mask X
		for (int c = 1; c < src.cols - 1; c++) {
			int w = 0; 	// index for weightx[]
			int wsum = 0; 	// for weighted sum
			for (int p = -1; p <= 1; p++) // convolution : weighted sum
				for (int q = -1; q <= 1; q++)
					wsum += src.at<uchar>(r + p, c + q) * weightx[w++];
			grad_x.at<short>(r, c) = wsum;
		}
	for (int r = 1; r < src.rows - 1; r++) // for sobel mask Y
		for (int c = 1; c < src.cols - 1; c++) {
			int w = 0; // index for weighty[]
			int wsum = 0; // for weighted sum
			for (int p = -1; p <= 1; p++) // convolution : weighted sum
				for (int q = -1; q <= 1; q++)
					wsum += src.at<uchar>(r + p, c + q) * weighty[w++];
			grad_y.at<short>(r, c) = wsum;
		}
	Mat abs_grad_x(src.rows, src.cols, CV_8U);
	Mat abs_grad_y(src.rows, src.cols, CV_8U);
	grad_x.convertTo(abs_grad_x, CV_8U); // convert 16 bit back to 8 bit
	grad_y.convertTo(abs_grad_y, CV_8U);
	//Calculate the gradient by adding both directional gradients
	Mat grad = Mat(src.rows, src.cols, CV_8U);
	grad = 0.5 * abs_grad_x + 0.5 * abs_grad_y;
	imshow("Sobel Pixels", grad);
	
}

void LaplacianPixels(Mat src)
{

	Mat grad_x(src.rows, src.cols, CV_16S);
	Mat grad_y(src.rows, src.cols, CV_16S);
	int weightx[] = { -1,-1,-1, -1,8,-1, -1,-1,-1 };
	int weighty[] = { -1,-1,-1, -1,8,-1, -1,-1,-1 };
	

	for (int r = 1; r < src.rows - 1; r++)  // for sobel mask X
		for (int c = 1; c < src.cols - 1; c++) {
			int w = 0; 	// index for weightx[]
			int wsum = 0; 	// for weighted sum
			for (int p = -1; p <= 1; p++) // convolution : weighted sum
				for (int q = -1; q <= 1; q++)
					wsum += src.at<uchar>(r + p, c + q) * weightx[w++];
			grad_x.at<short>(r, c) = wsum;
		}
	for (int r = 1; r < src.rows - 1; r++) // for sobel mask Y
		for (int c = 1; c < src.cols - 1; c++) {
			int w = 0; // index for weighty[]
			int wsum = 0; // for weighted sum
			for (int p = -1; p <= 1; p++) // convolution : weighted sum
				for (int q = -1; q <= 1; q++)
					wsum += src.at<uchar>(r + p, c + q) * weighty[w++];
			grad_y.at<short>(r, c) = wsum;
		}
	Mat abs_grad_x(src.rows, src.cols, CV_8U);
	Mat abs_grad_y(src.rows, src.cols, CV_8U);
	grad_x.convertTo(abs_grad_x, CV_8U); // convert 16 bit back to 8 bit
	grad_y.convertTo(abs_grad_y, CV_8U);
	//Calculate the gradient by adding both directional gradients
	Mat grad = Mat(src.rows, src.cols, CV_8U);
	grad = 0.5 * abs_grad_x + 0.5 * abs_grad_y;
	imshow("Laplacian Pixels", grad);

}



int main()
{
	// 원본 & 히스토그램 출력 
	Mat src = imread("MyPhoto2.jpg", IMREAD_COLOR);
	Mat dst, sharpeningDst;

	if (src.empty())	return -1;

	resize(src, src, Size(480, 480));
	imshow("MyPhoto", src);

	// (3-1) Edge Detection - 1차 미분 “Sobel” 
	Mat grad;
	int scale = 1;
	int delta = 0;

	Mat src2 = imread("MyPhoto2.jpg", IMREAD_GRAYSCALE);
	if (src.empty()) { return -1; }

	resize(src2, src2, Size(480, 480));
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;


	while (1) {
		int key = waitKeyEx();   // 사용자로부터 키를 기다린다. 
		cout << key << " ";
		if (key == '1') // Blurring & Sharpening
		{
			// (1) Gaussian Filter 를 이용한 "Blurring" 
			for (int i = 1; i < 7; i = i + 2)
			{
				GaussianBlur(src, dst, Size(i, i), 0, 0);
				//waitKey(1000);
			}
			imshow("Blurring", dst);

			// (2) "Sharpening"
			float weights[9] = { -1, -1, -1, -1, 9, -1, -1, -1, -1 };

			Mat mask = Mat(3, 3, CV_32F, weights);

			filter2D(dst, sharpeningDst, -1, mask, Point(-1, -1), 0, BORDER_DEFAULT);

			imshow("Sharpening", sharpeningDst);

		}
		else if (key == '2')
		{
			// (3-1) Edge Detection - 1차 미분 “Sobel” 
			
			// Sobel convolution
			Sobel(src2, grad_x, CV_16S, 1, 0, 3, scale, delta, BORDER_DEFAULT);
			Sobel(src2, grad_y, CV_16S, 0, 1, 3, scale, delta, BORDER_DEFAULT);

			//Convert output back to a CV_8U image
			convertScaleAbs(grad_x, abs_grad_x);
			convertScaleAbs(grad_y, abs_grad_y);

			//Approximate the gradient by adding both directional gradients
			addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

			
			resize(grad, grad, Size(480, 480));

			//imshow("MyPhoto2", src2);
			imshow("Sobel", grad);
		}
		else if (key == '3') 
		{
			// (3-2) Edge Detection - 1차 미분 “Sobel Pixels”

			SobelPixels(src2);
		}
		else if (key == '4')
		{
			// (4-1) Edge Detection - 2차 미분 “Laplacian”
			Mat laplacianDst;

			int kernel_size = 3;
			int ddepth = CV_16S;

			//GaussianBlur (InputArray src, OutputArray dst, Size ksize, double sigmaX, double sigmaY=0, int borderType=BORDER_DEFAULT)
			GaussianBlur(src2, src2, Size(3, 3), 0, 0, BORDER_DEFAULT);
			Mat abs_dst;

			// Laplacian( src_gray, dst, ddepth, kernel_size, scale=1, delta=0, BORDER_DEFAULT );
			Laplacian(src2, laplacianDst, ddepth, kernel_size, scale, delta, BORDER_DEFAULT);
			//Convert output back to a CV_8U image
			convertScaleAbs(laplacianDst, abs_dst);

			imshow("Laplacian", abs_dst);
		}
		else if (key == '5')
		{
			// (4-2) Edge Detection - 2차 미분 “Laplacian Pixels”

			LaplacianPixels(src2);
		}
	}



	waitKey();
	return 0;
}
