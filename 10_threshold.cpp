#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(void) {
	// response値の最小・最大を取得
	double response_min = 0;
	double response_max = 255;

	Mat image = imread("img/image.jpg", 0);

	// Create window
	const string window_name = "Threshold";
	namedWindow(window_name, 1);

	// Create trackbar to change threshold
	int trackbar_value = response_min;
	createTrackbar("threshold", window_name, &trackbar_value, response_max);

	while (true) {
		Mat output_image;
		int Threshold = getTrackbarPos("threshold", window_name);
		
		threshold(image, output_image, Threshold, 255, THRESH_BINARY);
		imshow("bainarization", output_image);

		// Escで終了
		if (waitKey(50) == 27) {
			imwrite("img/binarization_image.jpg", output_image);
			break;
		}
	}
	return 0;
}