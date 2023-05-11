#include <stdio.h>
#include <opencv2/opencv.hpp>

int main(void) {
	cv::Mat image = cv::imread("img/image.jpg");;
	double degree = 45.0;
	double scale = 1;
	float width = image.cols;
	float height = image.rows;

	// エラー処理
	if (image.empty() == true) {
		printf("image is empty!");
		return 0;
	}

	cv::Point2f center = cv::Point2f((width / 2), (height / 2));
	cv::Mat change = cv::getRotationMatrix2D(center, degree, scale);
	cv::warpAffine(image, image, change, image.size(), cv::INTER_CUBIC, cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));
	// INTER_CUBIC: バイキュービック補間
	// ORDER_CONSTANT:  外挿する値

	cv::imshow("WarpAffine", image);
	cv::waitKey(0);
	cv::imwrite("img/affine.jpg", image);
	return 0;
}