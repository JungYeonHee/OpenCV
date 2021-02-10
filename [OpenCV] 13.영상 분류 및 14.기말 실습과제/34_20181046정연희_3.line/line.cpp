#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int main()
{
	VideoCapture cap("highway.mp4"); // real-time camera capture
	if (!cap.isOpened()) { cout << "file not found!" << endl; }
	while (1)
	{
		
		Mat frame;
		cap >> frame;
		
		imshow("frame", frame);

		Mat dst, cdst;
		Canny(frame, dst, 100, 200); // edge detection
		
		imshow("edge", dst);

		cvtColor(dst, cdst, COLOR_GRAY2BGR);
		vector<Vec4i> lines;
		HoughLinesP(dst, lines, 1, CV_PI / 180, 50, 5, 10);
		for (size_t i = 0; i < lines.size(); i++) {
			Vec4i l = lines[i];
			line(frame, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255, 0, 0), 2, LINE_AA);
		} //칼라 영상 위에 red 라인 그리기
		imshow("detected lines", frame);
		if (waitKey(30) >= 0) break;


	}

	return 0;
}
