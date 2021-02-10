#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int red, green, blue;
int drawing = false;

Mat gaussian;

Point2f inputp[4];

int i;
int mx1, my1, mx2, my2;   // ���콺�� ������ �簢���� ��ǥ
bool cropping = false;

// �̹����� �� �׸��� ���콺 �ݹ��Լ� 
void onMouse(int event, int x, int y, int flags, void* param)
{
    Mat& img = *(Mat*)(param);

    if (event == EVENT_LBUTTONDOWN) {    // ���콺�� ���� ��ư�� ������
        mx1 = x;           // �簢���� ���� ��� ��ǥ ����
        my1 = y;
        cropping = true;
        circle(img, Point(x, y), 1, Scalar(0, 255, 0), 2); //����� 

        if (i < 4) {
            inputp[i] = Point2f(x, y);
            i++;
        }
        else
        {
            i = 0;
        }

    }
    else if (event == EVENT_LBUTTONUP) { // ���콺�� ���� ��ư���� ���� ����
        mx2 = x;            // �簢���� ���� �ϴ� ��ǥ ����
        my2 = y;
        cropping = false;
        rectangle(img, Rect(mx1, my1, mx2 - mx1, my2 - my1), Scalar(0, 255, 0), 2);

        imshow("src", img);

    }
    else if (event == EVENT_RBUTTONDOWN) {
        //circle(img, Point(x, y), 130, Scalar(0, 255, 255), 100);
        circle(img, Point(x, y), 60, Scalar(0, 0, 0), 120);
        Mat src;
        Mat clone = src.clone();


        //imwrite("gaussian.jpg", );
        imshow("src", img);   // ������ ����Ǹ� �ٽ� ǥ���Ѵ�.

    }
}

// Linear Interpolation
float Lerp(float s, float e, float t) {
    return s + (e - s) * t;
}

// BiLinear Interpolation
float Blerp(float c00, float c10, float c01, float c11, float tx, float ty) {
    return Lerp(Lerp(c00, c10, tx), Lerp(c01, c11, tx), ty);
}

// ���ο� ȭ�Ұ� ���
float GetPixel(Mat img, int x, int y, int c)
{
    if (x > 0 && y > 0 && x < img.cols && y < img.rows)
        return (float)(img.at<Vec3b>(y, x)[c]);
    else
        return 0.0;
}

// 1. Bilinear Interpolation

void interpolation() {

    //���� ���� ���� - �簢��
    Mat src = imread("lenna.jpg", IMREAD_COLOR);
    Mat clone = src.clone();
    imshow("image", src);
    Mat interpolation = clone(Rect(mx1, my1, mx2 - mx1, my2 - my1)); //x, y, width, height �ʷϻ� �ڽ� 
    imwrite("interpolation.jpg", interpolation); //�簢�� ������ ���� 

    cout << "(" << mx1 << "," << my1 << ") " << "(" << mx2 - mx1 << "," << my2 - my1 << ")";

    Mat dst = Mat::zeros(Size(interpolation.cols * 2, interpolation.rows * 2), interpolation.type()); //2�� �ø� �� 


    for (int y = 0; y < dst.rows; y++)
        for (int x = 0; x < dst.cols; x++) {
            for (int c = 0; c < 3; c++) { // for 3 channels
                float gx = ((float)x) / 2.0; //�ֺ� ȭ�� �ε��� ���
                float gy = ((float)y) / 2.0;
                int gxi = (int)gx;
                int gyi = (int)gy;
                float c00 = GetPixel(interpolation, gxi, gyi, c); // �ֺ� ȭ�Ұ� ����
                float c10 = GetPixel(interpolation, gxi + 1, gyi, c);
                float c01 = GetPixel(interpolation, gxi, gyi + 1, c);
                float c11 = GetPixel(interpolation, gxi + 1, gyi + 1, c);

                // �Ǽ����� ���� �κ� �����ϰ� 0�� 1������ ������
                int value = (int)Blerp(c00, c10, c01, c11, gx - gxi, gy - gyi);
                dst.at<Vec3b>(y, x)[c] = value;
            }
        }
    imwrite("dst.jpg", dst);

    //������ ������ ������ ��ġ�� ����
    Mat interpolationImg = imread("dst.jpg", IMREAD_COLOR); // 2�� �ø� �簢�� ���� ���� �̹��� 
    Mat interpolationRoi(src, Rect(mx1 - interpolation.cols / 2, my1 - interpolation.rows / 2, interpolationImg.cols, interpolationImg.rows)); // src�� �Ϻκп� �簢�� ������ ���ɿ��� ����ֱ� 
    interpolationImg.copyTo(interpolationRoi);

    imshow("Bilinear Interpolation ", src);
}

//2.Perspective Transformation
void transformation(Mat src) {

    //Mat src = imread("lenna.jpg", IMREAD_COLOR);
    Mat img = src.clone();

    // line(image, Point(100, 100), Point(300, 300), Scalar(0, 0, 255), 7);

    line(src, inputp[0], inputp[1], Scalar(0, 255, 0), 2);
    line(src, inputp[1], inputp[3], Scalar(0, 255, 0), 2);
    line(src, inputp[3], inputp[2], Scalar(0, 255, 0), 2);
    line(src, inputp[2], inputp[0], Scalar(0, 255, 0), 2);

    imshow("Draw Image", src);

    cout << endl;
    cout << "(" << inputp[0].x << "," << inputp[0].y << ")" << endl;
    cout << "(" << inputp[1].x << "," << inputp[1].y << ")" << endl;
    cout << "(" << inputp[2].x << "," << inputp[2].y << ")" << endl;
    cout << "(" << inputp[3].x << "," << inputp[3].y << ")" << endl;

    Point2f outputp[4]; //��� 
    outputp[0] = Point2f(0, 0);
    outputp[1] = Point2f(0, img.rows);
    outputp[2] = Point2f(img.cols, 0);
    outputp[3] = Point2f(img.cols, img.rows);
    Mat h = getPerspectiveTransform(inputp, outputp); // Perspective ��ķ� ��ȯ
    Mat out;
    warpPerspective(img, out, h, src.size());
    imshow("Source Image", img);
    imshow("Perspective", out);

}


// 3.Scale & Rotation Transformation
void rotation(Mat src) {

    //Mat src = imread("lenna.jpg", IMREAD_COLOR);
    Mat dst = Mat();
    Size dsize = Size(src.cols, src.rows);
    Point center = Point(src.cols / 2.0, src.rows / 2.0); //ȸ�� �߽�
    Mat M = getRotationMatrix2D(center, 45, 1.0); //ȸ�� ��� ���
    warpAffine(src, dst, M, dsize, INTER_LINEAR); //ȸ�� ���� dsize - ���ȭ�� 
    resize(dst, dst, Size(dst.cols / 2, dst.rows / 2));
    imshow("perspective transformation", dst);
}





// 4.WarpingY
void warpingY() {


    Mat src = imread("lenna.jpg", IMREAD_COLOR);

    int rows = src.rows;
    int cols = src.cols;

    Mat dst1 = src.clone();
    Mat dst2 = src.clone();
    Mat dst3 = src.clone();
    Mat dst4 = src.clone();

    for (int i = 0; i < rows; i++) { // y ����
        for (int j = 0; j < cols; j++) { // x ����
            for (int c = 0; c < 3; c++) { // for 3 channels
                int offset_x = 0;
                int offset_y1 = (int)(25.0 * sin(2 * 3.14 * j / rows)); // sine wave
                int offset_y2 = (int)(25.0 * sin(2 * 3.14 * j / 90)); // sine wave
                int offset_y3 = (int)(50.0 * sin(2 * 3.14 * j / 180)); // sine wave

                dst1.at<Vec3b>(i, j)[c] = src.at<Vec3b>((i + offset_y1) % rows, j)[c];
                dst2.at<Vec3b>(i, j)[c] = src.at<Vec3b>((i + offset_y2) % rows, j)[c];
                dst3.at<Vec3b>(i, j)[c] = src.at<Vec3b>((i + offset_y3) % rows, j)[c];

            }
        }
    }

    imshow("WapingY1", dst1);
    imshow("WapingY2", dst2);
    imshow("WapingY3", dst3);

}

// 5.Warping Advanced
void wapingAdvanced(Mat img) {

    resize(img, img, Size(399, 399)); // ���� �������� (400, 400) ���� �ϰ� �Ǹ� �� ���� �𸣰����� �Լ� ����� �ڵ尡 �׽��ϴ�.. �׷��� ����� �������־����ϴ�! ������ �밢������ sine wave�� �ִ� ���� �ٸ� �ڵ忡�� ��������� �� �ǹǷ� �밢�� sine wave �ڵ�� �½��ϴ�!! 
    Mat srcs = img.clone();
    Mat dstd = img.clone();

    int rows = dstd.rows;
    int cols = dstd.cols;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            for (int c = 0; c < 3; c++) {
                int offset_x = 0;
                int offset_y = (int)(25.0 * sin(2 * 3.14 * j / 180) - j);
                srcs.at<Vec3b>(i, j)[c] = dstd.at<Vec3b>((i + offset_y) % dstd.rows, j)[c];
            }
        }
    }

    imshow("Waping Advanced", srcs);
}


// 6. Gaussian Blurring

void gaussianBlurring(Mat src) {

    //Mat src2 = imread("lenna.jpg", IMREAD_COLOR);
    Mat dst1 = src.clone();

    threshold(dst1, dst1, 0, 255, THRESH_BINARY);
    // imshow("dst1", dst1); // ������ ���� ���� mask 
    imwrite("circle.jpg", dst1);
    Mat circle = imread("circle.jpg", IMREAD_COLOR);

    Mat blur = imread("lenna.jpg", IMREAD_COLOR);
    Mat dst2;

    for (int i = 1; i < 61; i = i + 2)
    {
        GaussianBlur(blur, dst2, Size(i, i), 0, 0); //src - dst1, out - dst2 
    }

    bitwise_and(dst2, circle, blur);
    imshow("Gaussian Blurring", blur);
}

// 7.
void segmentation() {
    // ���� & ������׷� ��� 
    Mat flower = imread("flower.jpg", IMREAD_COLOR);  // ���� �ε�

    resize(flower, flower, Size(400, 400));

    vector<Mat> bgr_planes;   // ����(Mat)���� ���� bgr_planes[0], [1], [2]
    split(flower, bgr_planes);   // �Է� ������ ���󺰷� �и��Ѵ�. 

    int histSize = 256;   // ������׷� ���� �׷����� ����
    float range[] = { 0, 256 };    // ȭ�Ұ� ����
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

    imshow("���� ����", flower);
    imshow("������׷� ���� ���", histImage);

    Mat dst = imread("flower.jpg", IMREAD_GRAYSCALE);
    //Mat dst;
    //cvtColor(flower, dst, cv::COLOR_BGRA2GRAY);;
    resize(dst, dst, Size(400, 400));

    Mat threshold_image;
    threshold(dst, threshold_image, 0, 255, THRESH_BINARY | THRESH_OTSU);
    imwrite("dst.jpg", threshold_image);

    Mat img = imread("lenna.jpg", IMREAD_COLOR);

    Mat mask = imread("dst.jpg", IMREAD_COLOR);
    imshow("Segmentation ��� ���� ���", mask);

    Mat img1 = img.clone();

    bitwise_and(img, mask, img1);
    imshow("Segmentation ��� ���� �� �� ��ģ ���� ���", img1);








}
int main()
{
    Mat src = imread("lenna2.jpg", IMREAD_COLOR);
    resize(src, src, Size(400, 400));
    if (src.empty()) { cout << "������ ���� �� ����" << endl; }
    imshow("src", src);


    setMouseCallback("src", onMouse, &src); // �ʷϻ� �簢�� �׸��� 

    while (1) {
        int key = waitKeyEx();   // ����ڷκ��� Ű�� ��ٸ���. 
        cout << key << " ";
        if (key == 'i')
        {
            interpolation(); // (interpolation)
        }
        else if (key == 'p') { // (perspective transformation)
            transformation(src);
        }
        else if (key == 's') { // (perspective transformation)

            rotation(src);
        }
        else if (key == 'w') { // (warpingY)
            warpingY();
        }
        else if (key == 'a') { // (warping Advanced)
            wapingAdvanced(src);
        }
        else if (key == 'b') { // (Gaussian Blurring)
            gaussianBlurring(src);
        }
        else if (key == 'g') { // (segmentation)
            segmentation();
        }
    }


    waitKey(0);
    return 0;

}
