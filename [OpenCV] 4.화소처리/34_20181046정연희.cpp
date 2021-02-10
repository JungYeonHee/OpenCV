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

//�̹����� �� �׸��� ���콺 �ݹ��Լ� 
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
    cout << "��Ÿ���� �Է��Ͻÿ�: [0-100]: ";
    cin >> beta;
    for (int y = 0; y < src.rows; y++) {
        for (int x = 0; x < src.cols; x++) {
            for (int c = 0; c < 3; c++) {
                oimage.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(alpha * (src.at<Vec3b>(y, x)[c]) + beta);
            }
        }
    }
    imshow("BRIGHT", oimage);   // ������ ����Ǿ����Ƿ� �ٽ� ǥ���Ѵ�.

}

void contrast()
{
    oimage = Mat::zeros(src.size(), src.type());
    cout << "���İ��� �Է��Ͻÿ�: [1.0-3.0]: ";
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

    cout << "�Ӱ谪�� �Է��Ͻÿ�: ";
    cin >> threshold_value;

    threshold(oimage, oimage, threshold_value, 255, THRESH_BINARY);
    imshow("BINARIZATION", oimage);
}

void gamma()
{
    double gamma = 1;
    cout << "�������� �Է��Ͻÿ�: ";
    cin >> gamma;

    oimage = Mat::zeros(src.size(), src.type());

    Mat table(1, 256, CV_8U);
    uchar* p = table.ptr(); // size 255 �迭
    for (int i = 0; i < 256; ++i)
        p[i] = saturate_cast <uchar> (pow(i / 255.0, gamma) * 255.0);
    LUT(src, table, oimage);

    imshow("GAMMA CORRECTION", oimage);

}

void facestagram()
{
    Mat mask, img1;

    img1 = imread("yeonhee.png", IMREAD_COLOR); // ���� �̹��� �ε� 
    if (img1.empty()) { cout << "����1�� �ε��� �� �����ϴ�." << endl; }

    threshold(heart, heart, 0, 255, THRESH_BINARY); // ����ȭ (���� �κ� ������ ������ �Ͼ������ ����)

    mask = 255 - heart; //���� ( ���� �κ� --> �Ͼ�� / �Ͼ� �κ� --> ������ ) 
    imwrite("dst.jpg", mask); // �÷� �̹����� �ٽ� �ε��ϱ� ���� �̹��� ���� 

    dst = imread("dst.jpg", IMREAD_COLOR); // �÷� �̹��� Ÿ������ �ε� 

    oimage = img1.clone();

    //imshow("img1", img1);
    //imshow("mask", dst);

    bitwise_and(img1, dst, oimage);
    imshow("#��Ÿ�׷�", oimage);
}
int main()
{

    src = imread("yeonhee.png", IMREAD_COLOR);
    if (src.empty()) { cout << "������ ���� �� ����" << endl; }


    imshow("My photo", src);

    // �̹����� �� �׸��� 
    setMouseCallback("My photo", onMouse, &src);

    // color image to �׷��̽�����
    //Mat gray, edge, output;
    //cvtColor(src, gray, cv::COLOR_BGRA2GRAY);

    //imshow("gray", gray);
    //imwrite("gray.jpg", gray);

    //Ʈ���� 
    namedWindow("My photo", 1);

    createTrackbar("R", "My photo", &red, 255, on_trackbar);
    createTrackbar("G", "My photo", &green, 255, on_trackbar);
    createTrackbar("B", "My photo", &blue, 255, on_trackbar);


    while (1) {
        int key = waitKeyEx();   // ����ڷκ��� Ű�� ��ٸ���. 
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
