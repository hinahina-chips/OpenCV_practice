#include <opencv2/opencv.hpp>

int main(void) {
	cv::Mat color_img = cv::imread("../image.jpg");;
	//エラー処理
	if (color_img.empty() == true) {
		// 画像の中身が空なら終了する
		printf("NULLerror!");
		return 0;
	}
	cv::Mat gray_img;
	// cvtColorを使ってグレースケール化
	cv::cvtColor(color_img, gray_img, cv::COLOR_BGR2GRAY);
	cv::imshow("grayscale", gray_img);
	cv::waitKey(0);
	return 0;
}
