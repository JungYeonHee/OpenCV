#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int red, green, blue;
int drawing = false;

Mat  roi, firResult, clone, src, firResultImg;



int mx1, my1, mx2, my2;   // 마우스로 지정한 사각형의 좌표
bool cropping = false;

//4. 이미지에 원 그리기 마우스 콜백함수 
void onMouse(int event, int x, int y, int flags, void* param)
{
    Mat& img = *(Mat*)(param);

    if (event == EVENT_LBUTTONDOWN) {    // 마우스의 왼쪽 버튼을 누르면
        mx1 = x;           // 사각형의 좌측 상단 좌표 저장
        my1 = y;
        cropping = true;
    }
    else if (event == EVENT_LBUTTONUP) { // 마우스의 왼쪽 버튼에서 손을 떼면
        mx2 = x;            // 사각형의 우측 하단 좌표 저장
        my2 = y;
        cropping = false;
        rectangle(img, Rect(mx1, my1, mx2 - mx1, my2 - my1), Scalar(0, 255, 0), 2);

        //관심 영역 지정 - 사각형
        clone = src.clone();

        firResult = clone(Rect(mx1, my1, mx2 - mx1, my2 - my1));
        imwrite("firResult.jpg", firResult);

        //지정한 영역을 정해진 위치에 삽입 - 첫번째 사진 삽입 
        firResultImg = imread("firResult.jpg", IMREAD_COLOR);
        Mat firstRoi(src, Rect(src.cols - firResultImg.cols - 10, 10, firResultImg.cols, firResultImg.rows));
        firResultImg.copyTo(firstRoi);

        //두번째 사진 삽입 
        resize(firResultImg, firResultImg, Size(firResultImg.cols / 2, firResultImg.rows / 2));
        Mat secRoi(src, Rect(src.cols - firResultImg.cols - 10, 10 + firResultImg.rows * 2, firResultImg.cols, firResultImg.rows));
        firResultImg.copyTo(secRoi);

        imshow("src", img);

    }
}

int main()
{
    src = imread("lenna.jpg", IMREAD_COLOR);
    Mat logo = imread("starbucks.jpg", IMREAD_COLOR);

    if (src.empty()) { cout << "영상을 읽을 수 없음" << endl; }
    imshow("src", src);

    // 도형 그리기 
    line(src, Point(100, 100), Point(300, 300), Scalar(0, 0, 255), 7);
    rectangle(src, Point(250, 30), Point(450, 200), Scalar(0, 255, 0), 5);
    circle(src, Point(100, 300), 60, Scalar(255, 0, 0), 3);
    ellipse(src, Point(300, 350), Point(100, 60), 45, 130, 270, Scalar(255, 255, 255), 5);

    //로고 삽입
    Mat roi(src, Rect(10, src.rows - logo.rows - 10, logo.cols, logo.rows));
    logo.copyTo(roi);

    imshow("src", src);


    setMouseCallback("src", onMouse, &src);

    // 만든 행렬 삽입 
    uchar data[10][60] = {
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };


    Mat logo2(10, 20, CV_8UC3, data);


    Mat roi2(src, Rect(src.cols - logo2.cols - 10, src.rows - logo2.rows - 10, logo2.cols, logo2.rows));
    logo2.copyTo(roi2);

    imshow("src", src);


    waitKey(0);
    return 0;

}

