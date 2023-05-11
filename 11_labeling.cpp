#include <opencv2/opencv.hpp>
#include<vector>

using namespace cv;

int main(void) {
	Mat image = imread("sample.png", 0);
	Mat output_image, bainari_image;
	Mat labels,bbox,center;
	threshold(image, bainari_image, 156, 255, THRESH_BINARY);
	int nlabel = connectedComponents(bainari_image, output_image);
	for (int i = 0; i < nlabel; i++) {
		//ラベルiを取り出す
		compare(output_image, i, bainari_image, CMP_EQ);
		//出力画像の保存
		imwrite("out" + std::to_string(i) + ".jpg", bainari_image);
		imshow("Labelling", bainari_image);
		waitKey(0);//入力待ち
	}
	return 0;
}
