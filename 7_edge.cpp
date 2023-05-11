#include <stdio.h>
#include <opencv2/opencv.hpp>

void sobel_filter(cv::Mat);
void laplacian_filter(cv::Mat);
void canny_alg(cv::Mat);

int main(void) {
	cv::Mat gray_image = cv::imread("img/image.jpg", 0);
	// edge_detect
	sobel_filter(gray_image);
	laplacian_filter(gray_image);
	canny_alg(gray_image);

	return 0;
}

//-----------------------------------------------------------------
void sobel_filter(cv::Mat image) {
	cv::Mat output_image;
	Sobel(image, output_image, CV_32F, 1, 0, 3);
	cv::convertScaleAbs(output_image, output_image, 1, 0);
	cv::imshow("sobel_output", output_image);
	cv::waitKey(0);
}

void laplacian_filter(cv::Mat image) {
	cv::Mat output_image;
	Laplacian(image, output_image, CV_32F, 1, 5);
	cv::convertScaleAbs(output_image, output_image, 1, 0);
	cv::imshow("laplacian", output_image);
	cv::waitKey(0);
}

void canny_alg(cv::Mat image) {
	cv::Mat output_image;
	Canny(image, output_image, 50, 100);
	cv::imshow("canny", output_image);
	cv::waitKey(0);
}
