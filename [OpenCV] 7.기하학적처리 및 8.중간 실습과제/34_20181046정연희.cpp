#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int red, green, blue;
int drawing = false;

Mat gaussian;

Point2f inputp[4];

int i;
int mx1, my1, mx2, my2;   // 마우스로 지정한 사각형의 좌표
bool cropping = false;

// 이미지에 원 그리기 마우스 콜백함수 
void onMouse(int event, int x, int y, int flags, void* param)
{
    Mat& img = *(Mat*)(param);

    if (event == EVENT_LBUTTONDOWN) {    // 마우스의 왼쪽 버튼을 누르면
        mx1 = x;           // 사각형의 좌측 상단 좌표 저장
        my1 = y;
        cropping = true;
        circle(img, Point(x, y), 1, Scalar(0, 255, 0), 2); //점찍기 

        if (i < 4) {
            inputp[i] = Point2f(x, y);
            i++;
        }
        else
        {
            i = 0;
        }

    }
    else if (event == EVENT_LBUTTONUP) { // 마우스의 왼쪽 버튼에서 손을 떼면
        mx2 = x;            // 사각형의 우측 하단 좌표 저장
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
        imshow("src", img);   // 영상이 변경되면 다시 표시한다.

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

// 새로운 화소값 계산
float GetPixel(Mat img, int x, int y, int c)
{
    if (x > 0 && y > 0 && x < img.cols && y < img.rows)
        return (float)(img.at<Vec3b>(y, x)[c]);
    else
        return 0.0;
}

// 1. Bilinear Interpolation

void interpolation() {

    //관심 영역 지정 - 사각형
    Mat src = imread("lenna.jpg", IMREAD_COLOR);
    Mat clone = src.clone();
    imshow("image", src);
    Mat interpolation = clone(Rect(mx1, my1, mx2 - mx1, my2 - my1)); //x, y, width, height 초록색 박스 
    imwrite("interpolation.jpg", interpolation); //사각형 지정한 영역 

    cout << "(" << mx1 << "," << my1 << ") " << "(" << mx2 - mx1 << "," << my2 - my1 << ")";

    Mat dst = Mat::zeros(Size(interpolation.cols * 2, interpolation.rows * 2), interpolation.type()); //2배 늘린 것 


    for (int y = 0; y < dst.rows; y++)
        for (int x = 0; x < dst.cols; x++) {
            for (int c = 0; c < 3; c++) { // for 3 channels
                float gx = ((float)x) / 2.0; //주변 화소 인덱스 계산
                float gy = ((float)y) / 2.0;
                int gxi = (int)gx;
                int gyi = (int)gy;
                float c00 = GetPixel(interpolation, gxi, gyi, c); // 주변 화소값 대입
                float c10 = GetPixel(interpolation, gxi + 1, gyi, c);
                float c01 = GetPixel(interpolation, gxi, gyi + 1, c);
                float c11 = GetPixel(interpolation, gxi + 1, gyi + 1, c);

                // 실수에서 정수 부분 제거하고 0과 1사이의 값으로
                int value = (int)Blerp(c00, c10, c01, c11, gx - gxi, gy - gyi);
                dst.at<Vec3b>(y, x)[c] = value;
            }
        }
    imwrite("dst.jpg", dst);

    //지정한 영역을 정해진 위치에 삽입
    Mat interpolationImg = imread("dst.jpg", IMREAD_COLOR); // 2배 늘린 사각형 지정 영역 이미지 
    Mat interpolationRoi(src, Rect(mx1 - interpolation.cols / 2, my1 - interpolation.rows / 2, interpolationImg.cols, interpolationImg.rows)); // src의 일부분에 사각형 지점에 관심영역 집어넣기 
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

    Point2f outputp[4]; //출력 
    outputp[0] = Point2f(0, 0);
    outputp[1] = Point2f(0, img.rows);
    outputp[2] = Point2f(img.cols, 0);
    outputp[3] = Point2f(img.cols, img.rows);
    Mat h = getPerspectiveTransform(inputp, outputp); // Perspective 행렬로 변환
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
    Point center = Point(src.cols / 2.0, src.rows / 2.0); //회전 중심
    Mat M = getRotationMatrix2D(center, 45, 1.0); //회전 행렬 계산
    warpAffine(src, dst, M, dsize, INTER_LINEAR); //회전 실행 dsize - 출력화면 
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

    for (int i = 0; i < rows; i++) { // y 방향
        for (int j = 0; j < cols; j++) { // x 방향
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

    resize(img, img, Size(399, 399)); // 원래 사이즈인 (400, 400) 으로 하게 되면 왜 인지 모르겠으나 함수 실행시 코드가 죽습니다.. 그래서 사이즈를 조절해주었습니다! 하지만 대각선으로 sine wave를 주는 것은 다른 코드에서 실행시켰을 때 되므로 대각선 sine wave 코드는 맞습니다!! 
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
    // imshow("dst1", dst1); // 검은색 원만 남은 mask 
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
    // 원본 & 히스토그램 출력 
    Mat flower = imread("flower.jpg", IMREAD_COLOR);  // 영상 로드

    resize(flower, flower, Size(400, 400));

    vector<Mat> bgr_planes;   // 영상(Mat)들의 벡터 bgr_planes[0], [1], [2]
    split(flower, bgr_planes);   // 입력 영상을 색상별로 분리한다. 

    int histSize = 256;   // 히스토그램 막대 그래프의 개수
    float range[] = { 0, 256 };    // 화소값 범위
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

    imshow("원본 영상", flower);
    imshow("히스토그램 영상 출력", histImage);

    Mat dst = imread("flower.jpg", IMREAD_GRAYSCALE);
    //Mat dst;
    //cvtColor(flower, dst, cv::COLOR_BGRA2GRAY);;
    resize(dst, dst, Size(400, 400));

    Mat threshold_image;
    threshold(dst, threshold_image, 0, 255, THRESH_BINARY | THRESH_OTSU);
    imwrite("dst.jpg", threshold_image);

    Mat img = imread("lenna.jpg", IMREAD_COLOR);

    Mat mask = imread("dst.jpg", IMREAD_COLOR);
    imshow("Segmentation 결과 영상 출력", mask);

    Mat img1 = img.clone();

    bitwise_and(img, mask, img1);
    imshow("Segmentation 결과 영상에 내 얼굴 합친 영상 출력", img1);








}
int main()
{
    Mat src = imread("lenna2.jpg", IMREAD_COLOR);
    resize(src, src, Size(400, 400));
    if (src.empty()) { cout << "영상을 읽을 수 없음" << endl; }
    imshow("src", src);


    setMouseCallback("src", onMouse, &src); // 초록색 사각형 그리기 

    while (1) {
        int key = waitKeyEx();   // 사용자로부터 키를 기다린다. 
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
