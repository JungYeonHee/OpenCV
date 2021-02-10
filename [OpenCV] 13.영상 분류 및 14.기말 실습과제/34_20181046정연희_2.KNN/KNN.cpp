#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;



int main()
{
    Mat img;
    img = imread("fashion.png", IMREAD_GRAYSCALE);
    namedWindow("original", WINDOW_AUTOSIZE);
    imshow("original", img);

    int key = waitKeyEx();

    if (key == '1')
    {
        Mat train_features(900, 576, CV_32FC1); // 24*24
        Mat labels(900, 1, CV_32FC1); // 레이블 10개 

        // 각 숫자 영상을 row vector로 만들어서 train_features에 저장한다.
        for (int r = 0; r < 30; r++) { // 숫자 세로로 30개
            for (int c = 0; c < 30; c++) { //숫자 가로로 30개
                int i = 0;
                for (int y = 0; y < 24; y++) { // 24x24 <-- 720/30 = 24 
                    for (int x = 0; x < 24; x++) {
                        train_features.at<float>(r * 30 + c, i++) = img.at<uchar>(r * 24 + y, c * 24 + x);
                    }
                }
            }
        }

        // 각 패션 영상에 대한 레이블을 저장한다.
        for (int i = 0; i < 900; i++) { // 숫자 0~9가 90개씩
            labels.at<float>(i, 0) = (i / 90);
        }

        // 학습 단계
        Ptr<ml::KNearest> knn = ml::KNearest::create();
        Ptr<ml::TrainData> trainData = ml::TrainData::create(train_features, cv::ml::SampleTypes::ROW_SAMPLE, labels);
        knn->train(trainData);

        float correct = 0;
        // 테스트 단계
        Mat predictedLabels;
        for (int i = 0; i < 900; i++) { // 이미지 900개에 대해서
            Mat test = train_features.row(i); //train 데이터를 test 데이터로 사용
            knn->findNearest(test, 3, predictedLabels); // k=3
            float prediction = predictedLabels.at<float>(0);

            if (i >= 0 && i < 90) {
                if (prediction == 0)
                    correct++;
            }
            else if (i >= 90 && i < 180) {
                if (prediction == 1)
                    correct++;
            }
            else if (i >= 180 && i < 270) {
                if (prediction == 2)
                    correct++;
            }
            else if (i >= 270 && i < 360) {
                if (prediction == 3)
                    correct++;
            }
            else if (i >= 360 && i < 450) {
                if (prediction == 4)
                    correct++;
            }
            else if (i >= 450 && i < 540) {
                if (prediction == 5)
                    correct++;
            }
            else if (i >= 540 && i < 630) {
                if (prediction == 6)
                    correct++;
            }
            else if (i >= 630 && i < 720) {
                if (prediction == 7)
                    correct++;
            }
            else if (i >= 720 && i < 810) {
                if (prediction == 8)
                    correct++;
            }
            else if (i >= 810 && i < 900) {
                if (prediction == 9)
                    correct++;
            }

            //string label 
            String pred;

            if (prediction == 0) {
                pred = "T-shirt";
            }
            else if (prediction == 1) {
                pred = "pants";
            }
            else if (prediction == 2) {
                pred = "shirt";
            }
            else if (prediction == 3) {
                pred = "dress";
            }
            else if (prediction == 4) {
                pred = "jacket";
            }
            else if (prediction == 5) {
                pred = "shoes";
            }
            else if (prediction == 6) {
                pred = "blouse";
            }
            else if (prediction == 7) {
                pred = "sneakers";
            }
            else if (prediction == 8) {
                pred = "bag";
            }
            else if (prediction == 9) {
                pred = "boots";
            }

            cout << "테스트 샘플" << i << "의 라벨 = " << pred << '\n';
        }

        correct = (float)(correct / 900.0) * 100.0;

        cout << "정답률: " << correct;


    }
    else if (key == '2') {
        Mat img;
        img = imread("fashion.png", IMREAD_GRAYSCALE);
        namedWindow("original", WINDOW_AUTOSIZE);
        imshow("original", img);
    
        Mat train_features(900, 576, CV_32F); // 24*24
        Mat labels(900, 1, CV_32F); // 레이블 10개 
 
        // 각 숫자 영상을 row vector로 만들어서 train_features에 저장한다.
        for (int r = 0; r < 30; r++) { // 숫자 세로로 30개
            for (int c = 0; c < 30; c++) { //숫자 가로로 30개
                int i = 0;
                for (int y = 0; y < 24; y++) { // 24x24 <-- 720/30 = 24 
                    for (int x = 0; x < 24; x++) {
                        train_features.at<float>(r * 30 + c, i++) = img.at<uchar>(r * 24 + y, c * 24 + x);
                    }
                }
            }
        }
 
        // 각 패션 영상에 대한 레이블을 저장한다.
        for (int i = 0; i < 900; i++) { // 숫자 0~9가 90개씩
            labels.at<float>(i, 0) = i/90;
        }

        // 학습 단계
        Ptr<ml::KNearest> knn = ml::KNearest::create();
        Ptr<ml::TrainData> trainData = ml::TrainData::create(train_features, cv::ml::SampleTypes::ROW_SAMPLE, labels);
        knn->train(trainData);


        float correct = 0;
        // test_img로 테스트 단계 
        Mat test_img;
        test_img = imread("jacket.png"); // test_img
        resize(test_img, test_img, Size(400, 400)); // 원본 이미지 사이즈 조절 
        imshow("test_img_0", test_img);

        cvtColor(test_img, test_img, COLOR_BGR2GRAY);
        resize(test_img, test_img, Size(24, 24)); // 24x24 이미지로 바꿈 

        namedWindow("test_img", WINDOW_AUTOSIZE);
        imshow("test_img", test_img);
        
        Mat test_features(1, 576, CV_32FC1); // 24 x 24 = 576

        int i = 0;
        for (int y = 0; y < 24; y++) { 
            for (int x = 0; x < 24; x++) {
                test_features.at<float>(0, i++)
                    = test_img.at<uchar>(y, x);
            }
        }

        Mat predictedLabels;
        for (int i = 0; i < 1; i++) { 
            Mat test = test_features.row(i); //test_features데이터를 test 데이터로 사용
            knn->findNearest(test, 71, predictedLabels); 
            float prediction = predictedLabels.at<float>(0);
            
            //string label 
            String pred;

            if (prediction == 0) {
                pred = "T-shirt";
            }
            else if (prediction == 1) {
                pred = "pants";
            }
            else if (prediction == 2) {
                pred = "shirt";
            }
            else if (prediction == 3) {
                pred = "dress";
            }
            else if (prediction == 4) {
                pred = "jacket";
            }
            else if (prediction == 5) {
                pred = "shoes";
            }
            else if (prediction == 6) {
                pred = "blouse";
            }
            else if (prediction == 7) {
                pred = "sneakers";
            }
            else if (prediction == 8) {
                pred = "bag";
            }
            else if (prediction == 9) {
                pred = "boots";
            }
      
            cout << "테스트 샘플" << i << "의 라벨 = " << pred << '\n';
        }

    }
    else if (key == '3') {
    Mat img;
    img = imread("fashion.png", IMREAD_GRAYSCALE);
  
    threshold(img, img, 127, 255, cv::THRESH_BINARY);

    Mat skele(img.size(), CV_8UC1, Scalar(0)); 
    Mat element = getStructuringElement(MORPH_CROSS, Size(3, 3));
    Mat tmp, eroded;

    do
    {
        erode(img, eroded, element);
        dilate(eroded, tmp, element);
        subtract(img, tmp, tmp); // outline 돌출 부분
        bitwise_or(skele, tmp, skele); // 기존의 skeleto에 합집합.
        eroded.copyTo(img);

    } while ((countNonZero(img) != 0));

    imshow("original", skele);

    Mat train_features(900, 576, CV_32FC1); // 24*24
    Mat labels(900, 1, CV_32FC1); // 레이블 10개 

    // 각 숫자 영상을 row vector로 만들어서 train_features에 저장한다.

    for (int r = 0; r < 30; r++) { // 숫자 세로로 30개
        for (int c = 0; c < 30; c++) { //숫자 가로로 30개
            int i = 0;
            for (int y = 0; y < 24; y++) { // 24x24 <-- (이미지크기 / 갯수) 720/30 = 24 
                for (int x = 0; x < 24; x++) {
                    train_features.at<float>(r * 30 + c, i++) = skele.at<uchar>(r * 24 + y, c * 24 + x);
                }
            }
        }
    }

    // 각 패션 영상에 대한 레이블을 저장한다.
    for (int i = 0; i < 900; i++) { // 숫자 0~9가 90개씩
        labels.at<float>(i, 0) = (i / 90);
    }

    // 학습 단계
    Ptr<ml::KNearest> knn = ml::KNearest::create();
    Ptr<ml::TrainData> trainData = ml::TrainData::create(train_features, cv::ml::SampleTypes::ROW_SAMPLE, labels);
    knn->train(trainData);

    float correct = 0;
    // 테스트 단계
    Mat predictedLabels;
    for (int i = 0; i < 900; i++) { // 이미지 900개에 대해서
        Mat test = train_features.row(i); //train 데이터를 test 데이터로 사용
        knn->findNearest(test, 3, predictedLabels); // k=3
        float prediction = predictedLabels.at<float>(0);
       
        if (i >= 0 && i < 90) {
            if (prediction == 0)
                correct++;
        }
        else if (i >= 90 && i < 180) {
            if (prediction == 1)
                correct++;
        }
        else if (i >= 180 && i < 270) {
            if (prediction == 2)
                correct++;
        }
        else if (i >= 270 && i < 360) {
            if (prediction == 3)
                correct++;
        }
        else if (i >= 360 && i < 450) {
            if (prediction == 4)
                correct++;
        }
        else if (i >= 450 && i < 540) {
            if (prediction == 5)
                correct++;
        }
        else if (i >= 540 && i < 630) {
            if (prediction == 6)
                correct++;
        }
        else if (i >= 630 && i < 720) {
            if (prediction == 7)
                correct++;
        }
        else if (i >= 720 && i < 810) {
            if (prediction == 8)
                correct++;
        }
        else if (i >= 810 && i < 900) {
            if (prediction == 9)
                correct++;
        }

        //string label 
        String pred;

        if (prediction == 0) {
            pred = "top";
        }
        else if (prediction == 1) {
            pred = "pants";
        }
        else if (prediction == 2) {
            pred = "top";
        }
        else if (prediction == 3) {
            pred = "dress";
        }
        else if (prediction == 4) {
            pred = "top";
        }
        else if (prediction == 5) {
            pred = "shoes";
        }
        else if (prediction == 6) {
            pred = "top";
        }
        else if (prediction == 7) {
            pred = "sneakers";
        }
        else if (prediction == 8) {
            pred = "bag";
        }
        else if (prediction == 9) {
            pred = "boots";
        }
        cout << "테스트 샘플" << i << "의 라벨 = " << pred << '\n';
    }

    correct = (float)(correct / 900.0) * 100.0;

    cout << "정답률: " << correct;

    }

    waitKey();


    return 0;
}