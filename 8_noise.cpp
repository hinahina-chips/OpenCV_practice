#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(void){
    // 画像の読み込み
    Mat image = imread("img/image.jpg", 0);

    // ノイズの付加
    Mat noise = Mat::zeros(image.size(), CV_8U);
    // 平均0、標準偏差50の正規分布からランダムな値を生成する
    randn(noise, 0, 50);
    
    // 入力画像とノイズを足し合わせる
    Mat noisy_image = image + noise;

    // ノイズ付き画像の保存
    cv::imshow("noise", noisy_image);
    cv::imwrite("img/noisy_output.jpg", noisy_image);
    cv::waitKey(0);

    return 0;
}