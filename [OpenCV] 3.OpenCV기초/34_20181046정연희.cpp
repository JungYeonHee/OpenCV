#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int red, green, blue;
int drawing = false;

Mat  roi, firResult, clone, src, firResultImg;



int mx1, my1, mx2, my2;   // ���콺�� ������ �簢���� ��ǥ
bool cropping = false;

//4. �̹����� �� �׸��� ���콺 �ݹ��Լ� 
void onMouse(int event, int x, int y, int flags, void* param)
{
    Mat& img = *(Mat*)(param);

    if (event == EVENT_LBUTTONDOWN) {    // ���콺�� ���� ��ư�� ������
        mx1 = x;           // �簢���� ���� ��� ��ǥ ����
        my1 = y;
        cropping = true;
    }
    else if (event == EVENT_LBUTTONUP) { // ���콺�� ���� ��ư���� ���� ����
        mx2 = x;            // �簢���� ���� �ϴ� ��ǥ ����
        my2 = y;
        cropping = false;
        rectangle(img, Rect(mx1, my1, mx2 - mx1, my2 - my1), Scalar(0, 255, 0), 2);

        //���� ���� ���� - �簢��
        clone = src.clone();

        firResult = clone(Rect(mx1, my1, mx2 - mx1, my2 - my1));
        imwrite("firResult.jpg", firResult);

        //������ ������ ������ ��ġ�� ���� - ù��° ���� ���� 
        firResultImg = imread("firResult.jpg", IMREAD_COLOR);
        Mat firstRoi(src, Rect(src.cols - firResultImg.cols - 10, 10, firResultImg.cols, firResultImg.rows));
        firResultImg.copyTo(firstRoi);

        //�ι�° ���� ���� 
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

    if (src.empty()) { cout << "������ ���� �� ����" << endl; }
    imshow("src", src);

    // ���� �׸��� 
    line(src, Point(100, 100), Point(300, 300), Scalar(0, 0, 255), 7);
    rectangle(src, Point(250, 30), Point(450, 200), Scalar(0, 255, 0), 5);
    circle(src, Point(100, 300), 60, Scalar(255, 0, 0), 3);
    ellipse(src, Point(300, 350), Point(100, 60), 45, 130, 270, Scalar(255, 255, 255), 5);

    //�ΰ� ����
    Mat roi(src, Rect(10, src.rows - logo.rows - 10, logo.cols, logo.rows));
    logo.copyTo(roi);

    imshow("src", src);


    setMouseCallback("src", onMouse, &src);

    // ���� ��� ���� 
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

