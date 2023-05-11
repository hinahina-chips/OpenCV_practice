#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

#define IMAGE_NUM  (3)         
#define PAT_ROW    (6)         
#define PAT_COL    (9)
#define PAT_SIZE   (PAT_ROW*PAT_COL)
#define ALL_POINTS (IMAGE_NUM*PAT_SIZE)
#define CHESS_SIZE (25.0)

int main(void) {
    int corner_count, found;
    int p_count[IMAGE_NUM];
    // cv::Mat src_img[IMAGE_NUM];
    vector<cv::Mat> srcImages;
    cv::Size pattern_size = cv::Size2i(PAT_COL, PAT_ROW);
    vector<cv::Point2f> corners;
    vector<vector<cv::Point2f>> img_points;

    // キャリブレーション画像の読み込み
    for (int i = 0; i < IMAGE_NUM; i++)
    {
        ostringstream ostr;
        ostr << "../calib_img\\" << setfill('0') << setw(1) << i << ".jpg";
        cv::Mat src = cv::imread(ostr.str());
        if (src.empty())
        {
            cerr << "cannot load image file : " << ostr.str() << endl;
        }
        else
        {
            srcImages.push_back(src);
        }
    }

    // 3次元空間座標の設定

    vector<cv::Point3f> object;
    for (int j = 0; j < PAT_ROW; j++)
    {
        for (int k = 0; k < PAT_COL; k++)
        {
            cv::Point3f p(
                j * CHESS_SIZE,
                k * CHESS_SIZE,
                0.0);
            object.push_back(p);
        }
    }

    vector<vector<cv::Point3f>> obj_points;
    for (int i = 0; i < IMAGE_NUM; i++)
    {
        obj_points.push_back(object);
    }

    // ３次元の点を ALL_POINTS * 3 の行列(32Bit浮動小数点数:１チャンネル)に変換する 


    // チェスボード（キャリブレーションパターン）のコーナー検出
    int found_num = 0;
    cv::namedWindow("Calibration", cv::WINDOW_AUTOSIZE);
    for (int i = 0; i < IMAGE_NUM; i++)
    {
        auto found = cv::findChessboardCorners(srcImages[i], pattern_size, corners);
        if (found)
        {
            cout << setfill('0') << setw(1) << i << "... ok" << endl;
            found_num++;
        }
        else
        {
            cerr << setfill('0') << setw(1) << i << "... fail" << endl;
        }

        // (4)コーナー位置をサブピクセル精度に修正，描画
        cv::Mat src_gray = cv::Mat(srcImages[i].size(), CV_8UC1);
        cv::cvtColor(srcImages[i], src_gray, cv::COLOR_BGR2GRAY);
        cv::find4QuadCornerSubpix(src_gray, corners, cv::Size(3, 3));
        //cv::drawChessboardCorners(srcImages[i], pattern_size, corners, found);
        img_points.push_back(corners);

    }
    // 位置推定
    cv::Mat rvec_est;
    cv::Mat tvec_est;

    //TODO insric, distorsionをファイルからとってくる
    cv::Mat intrinsic;
    cv::Mat distortion;
    std::string  fileName1 = "../kadai12/camera.xml";
    cv::FileStorage tcvfs(fileName1, cv::FileStorage::READ);
    cv::read(tcvfs["intrinsic"], intrinsic);
    cv::read(tcvfs["distortion"], distortion);
    tcvfs.release();
    if (intrinsic.cols != 3) {
        cout << "can't read camera parameter: " << fileName1 << endl;
        getchar();
    }
    cv::solvePnP(obj_points[2], img_points[2], intrinsic, distortion, rvec_est, tvec_est);
    cv::imshow("test", srcImages[1]);


    //-----------------------------------------
    float length = 100;
    cv::Point3f origin(0, 0, 0);
    cv::Mat xAxis = (cv::Mat_<double>(3, 1) << length, 0, 0);
    cv::Mat yAxis = (cv::Mat_<double>(3, 1) << 0, length, 0);
    cv::Mat zAxis = (cv::Mat_<double>(3, 1) << 0, 0, length);

    std::vector<cv::Point3f> axisPoints;
    axisPoints.push_back(origin);
    axisPoints.push_back(cv::Point3f(xAxis));
    axisPoints.push_back(cv::Point3f(yAxis));
    axisPoints.push_back(cv::Point3f(zAxis));


    // Rotate and translate the endpoints based on the rvec and tvec
    cv::Mat rotMat;
    cv::Rodrigues(rvec_est, rotMat);

    cv::Mat xEnd = rotMat * xAxis + tvec_est;
    cv::Mat yEnd = rotMat * yAxis + tvec_est;
    cv::Mat zEnd = rotMat * zAxis + tvec_est;

    // Add the endpoints to the list of axis points
    axisPoints.push_back(cv::Point3f(xEnd));
    axisPoints.push_back(cv::Point3f(yEnd));
    axisPoints.push_back(cv::Point3f(zEnd));

    // Project the points onto the image plane
    std::vector<cv::Point2f> img_points2;
    cv::projectPoints(axisPoints, rvec_est, tvec_est, intrinsic, distortion, img_points2);
    cv::line(srcImages[2], img_points2[0], img_points2[1], cv::Scalar(0, 0, 255), 2); // X-axis (red)
    cv::line(srcImages[2], img_points2[0], img_points2[2], cv::Scalar(0, 255, 0), 2); // Y-axis (green)
    cv::line(srcImages[2], img_points2[0], img_points2[3], cv::Scalar(255, 0, 0), 2); // Z-axis (blue)
    cv::imshow("test", srcImages[2]);
    cv::waitKey(0);
}
