#include <opencv2/opencv.hpp>
#include <iostream>
#include <direct.h>

// Debugのみでしか動かなかった
// ffmegが必要だと怒られた
int main(void){
    cv::Mat image;
    std::string filepath = "movie/video.mp4";
    std::string foldername = "img/images";
   
    // 連番画像を入れるフォルダが無ければ作成
    if (_mkdir(foldername.c_str()) == 0) {
        printf("フォルダの作成が成功しました!");
    }
    //動画をキャプチャ
    cv::VideoCapture video;
    video.open(filepath);
    if (video.isOpened() == false) {
        printf("動画をキャプチャ出来ませんでした");
        return 0;
    }

    for (int i = 0; i < 100; i++) {

        // 1フレーム分取り出してimgに保持させる
        video >> image;

        // 現在のフレーム番号を表示
        std::cout << "フレーム番号 " << i << std::endl;

        // 画像を表示
        cv::imshow("Video", image);

        // 取得した画像を連番画像で保存
        // 3桁で空いている桁は0で埋める
        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(3) << i;
        cv::imwrite(foldername + "/" + oss.str() + ".jpg", image);

        // 表示のために30ms待つ
        cv::waitKey(30);
    }
    return 0;
}