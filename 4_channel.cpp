#include <stdio.h>
#include <opencv2/opencv.hpp>

int main(void) {
	cv::Mat image, output_image;

	image = cv::imread("img/image.jpg");
	cv::cvtColor(image, output_image, cv::COLOR_BGR2RGB);
	cv::imshow("test", output_image);
	cv::waitKey(0);
	return 0;
}