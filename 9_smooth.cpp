#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(void){
    // 画像の読み込み
    Mat image = imread("img/noisy_output.jpg", IMREAD_GRAYSCALE);

    // ホワイトノイズの除去
    Mat denoised_image;
    GaussianBlur(
        image,
        denoised_image,
        Size(3, 3),
        0
    );
    // ノイズ除去後の画像の保存
    imshow("denoise", denoised_image);
    waitKey(0);
    imwrite("img/denoised_output.jpg", denoised_image);

    return 0;
}