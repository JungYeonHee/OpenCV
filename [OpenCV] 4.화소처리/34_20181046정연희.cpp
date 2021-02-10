#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int red, green, blue;
int drawing = false;

double alpha = 1.0;
int beta = 0;

Mat src, oimage, heart, dst;


void on_trackbar(int, void*) {     }

//이미지에 원 그리기 마우스 콜백함수 
void onMouse(int event, int x, int y, int flags, void* param)
{
    oimage = Mat::zeros(src.size(), src.type());
    Mat& img = *(Mat*)(param);
    if (event == EVENT_LBUTTONDOWN)
        drawing = true;
    else if (event == EVENT_MOUSEMOVE) {
        if (drawing == true)
            circle(img, Point(x, y), 3, Scalar(blue, green, red), 10);
        imwrite("heart.jpg", src);
        cvtColor(src, heart, cv::COLOR_BGRA2GRAY);
    }
    else if (event == EVENT_LBUTTONUP)
        drawing = false;

    imshow("My photo", img);
}

void brighten()
{

    oimage = Mat::zeros(src.size(), src.type());
    cout << "베타값을 입력하시오: [0-100]: ";
    cin >> beta;
    for (int y = 0; y < src.rows; y++) {
        for (int x = 0; x < src.cols; x++) {
            for (int c = 0; c < 3; c++) {
                oimage.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(alpha * (src.at<Vec3b>(y, x)[c]) + beta);
            }
        }
    }
    imshow("BRIGHT", oimage);   // 영상이 변경되었으므로 다시 표시한다.

}

void contrast()
{
    oimage = Mat::zeros(src.size(), src.type());
    cout << "알파값을 입력하시오: [1.0-3.0]: ";
    cin >> alpha;

    for (int y = 0; y < src.rows; y++) {
        for (int x = 0; x < src.cols; x++) {
            for (int c = 0; c < 3; c++) {
                oimage.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(alpha * (src.at<Vec3b>(y, x)[c]) + beta);
            }
        }
    }
    imshow("CONTRAST", oimage);

}

void invert()
{
    oimage = Mat::zeros(src.size(), src.type());
    for (int y = 0; y < src.rows; y++) {
        for (int x = 0; x < src.cols; x++) {
            for (int c = 0; c < 3; c++) {
                oimage.at<Vec3b>(y, x)[c] = 255 - saturate_cast<uchar>(alpha * (src.at<Vec3b>(y, x)[c]) + beta);
            }
        }
    }
    imshow("INVERT", oimage);
}

void threshold()
{
    int threshold_value = 0;
    oimage = Mat::zeros(src.size(), src.type());
    cvtColor(src, oimage, cv::COLOR_BGRA2GRAY);

    cout << "임계값을 입력하시오: ";
    cin >> threshold_value;

    threshold(oimage, oimage, threshold_value, 255, THRESH_BINARY);
    imshow("BINARIZATION", oimage);
}

void gamma()
{
    double gamma = 1;
    cout << "감마값을 입력하시오: ";
    cin >> gamma;

    oimage = Mat::zeros(src.size(), src.type());

    Mat table(1, 256, CV_8U);
    uchar* p = table.ptr(); // size 255 배열
    for (int i = 0; i < 256; ++i)
        p[i] = saturate_cast <uchar> (pow(i / 255.0, gamma) * 255.0);
    LUT(src, table, oimage);

    imshow("GAMMA CORRECTION", oimage);

}

void facestagram()
{
    Mat mask, img1;

    img1 = imread("yeonhee.png", IMREAD_COLOR); // 원본 이미지 로드 
    if (img1.empty()) { cout << "영상1을 로드할 수 없습니다." << endl; }

    threshold(heart, heart, 0, 255, THRESH_BINARY); // 이진화 (검정 부분 제외한 나머지 하얀색으로 만듬)

    mask = 255 - heart; //반전 ( 검정 부분 --> 하얀색 / 하얀 부분 --> 검정색 ) 
    imwrite("dst.jpg", mask); // 컬러 이미지로 다시 로드하기 위해 이미지 저장 

    dst = imread("dst.jpg", IMREAD_COLOR); // 컬러 이미지 타입으로 로드 

    oimage = img1.clone();

    //imshow("img1", img1);
    //imshow("mask", dst);

    bitwise_and(img1, dst, oimage);
    imshow("#얼스타그램", oimage);
}
int main()
{

    src = imread("yeonhee.png", IMREAD_COLOR);
    if (src.empty()) { cout << "영상을 읽을 수 없음" << endl; }


    imshow("My photo", src);

    // 이미지에 원 그리기 
    setMouseCallback("My photo", onMouse, &src);

    // color image to 그레이스케일
    //Mat gray, edge, output;
    //cvtColor(src, gray, cv::COLOR_BGRA2GRAY);

    //imshow("gray", gray);
    //imwrite("gray.jpg", gray);

    //트랙바 
    namedWindow("My photo", 1);

    createTrackbar("R", "My photo", &red, 255, on_trackbar);
    createTrackbar("G", "My photo", &green, 255, on_trackbar);
    createTrackbar("B", "My photo", &blue, 255, on_trackbar);


    while (1) {
        int key = waitKeyEx();   // 사용자로부터 키를 기다린다. 
        cout << key << " ";
        if (key == '1')
        {
            brighten();
        }
        else if (key == '2')
        {
            contrast();
        }
        else if (key == '3')
        {
            invert();
        }
        else if (key == '4')
        {
            threshold();
        }
        else if (key == '5')
        {
            gamma();
        }
        else if (key == '6')
        {
            facestagram();
        }
    }

    waitKey(0);
    return 0;

}
