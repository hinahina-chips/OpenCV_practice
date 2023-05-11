#include <opencv2/opencv.hpp>

int main(void) {
	// Mat形式で格納
	cv::Mat image = cv::imread("img/image.jpg");

	cv::imshow("test", image);
	// キーを押すまで待機
	cv::waitKey(0);
	return 0;
}
