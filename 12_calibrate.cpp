#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/cvconfig.h>
#include <opencv2/imgcodecs.hpp>
#include <vector>


using namespace std;

#define IMAGE_NUM  (4)         /* �摜�� */
#define PAT_ROW    (6)          /* �p�^�[���̍s�� */
#define PAT_COL    (9)         /* �p�^�[���̗� */
#define PAT_SIZE   (PAT_ROW*PAT_COL)
#define ALL_POINTS (IMAGE_NUM*PAT_SIZE)
#define CHESS_SIZE (25.0)       /* �p�^�[��1�}�X��1�ӃT�C�Y[mm] */

int main(int argc, char* argv[])
{
    int i, j, k;
    int corner_count, found;
    int p_count[IMAGE_NUM];
    // cv::Mat src_img[IMAGE_NUM];
    vector<cv::Mat> srcImages;
    cv::Size pattern_size = cv::Size2i(PAT_COL, PAT_ROW);
    vector<cv::Point2f> corners;
    vector<vector<cv::Point2f>> img_points;

    // �L�����u���[�V�����摜�̓ǂݍ���
    for (i = 0; i < IMAGE_NUM; i++)
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

    // 3������ԍ��W�̐ݒ�

    vector<cv::Point3f> object;
    for (j = 0; j < PAT_ROW; j++)
    {
        for (k = 0; k < PAT_COL; k++)
        {
            cv::Point3f p(
                j * CHESS_SIZE,
                k * CHESS_SIZE,
                0.0);
            object.push_back(p);
        }
    }

    vector<vector<cv::Point3f>> obj_points;
    for (i = 0; i < IMAGE_NUM; i++)
    {
        obj_points.push_back(object);
    }

    // �R�����̓_�� ALL_POINTS * 3 �̍s��(32Bit���������_��:�P�`�����l��)�ɕϊ����� 


    // �`�F�X�{�[�h�i�L�����u���[�V�����p�^�[���j�̃R�[�i�[���o
    int found_num = 0;
    cv::namedWindow("Calibration", cv::WINDOW_AUTOSIZE);
    for (i = 0; i < IMAGE_NUM; i++)
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

        // (4)�R�[�i�[�ʒu���T�u�s�N�Z�����x�ɏC���C�`��
        cv::Mat src_gray = cv::Mat(srcImages[i].size(), CV_8UC1);
        cv::cvtColor(srcImages[i], src_gray, cv::COLOR_BGR2GRAY);
        cv::find4QuadCornerSubpix(src_gray, corners, cv::Size(3, 3));
        //cv::drawChessboardCorners(srcImages[i], pattern_size, corners, found);
        img_points.push_back(corners);

        cv::imshow("Calibration", srcImages[i]);
        cv::waitKey(0);
    }


    if (found_num != IMAGE_NUM)
    {
        cerr << "Calibration Images are insufficient." << endl;
        return -1;
    }

    // (5)�����p�����[�^�C�c�݌W���̐���
    cv::Mat intrinsic; // �J���������p�����[�^�s��
    cv::Mat distortion; // �c�݌W��
    vector<cv::Mat> rvecs, tvecs; // �e�r���[�̉�]�x�N�g���ƕ��i�x�N�g��
    cv::calibrateCamera(
        obj_points,
        img_points,
        srcImages[0].size(),
        intrinsic,
        distortion,
        rvecs,
        tvecs
    );
    // ���e�ɂ��덷�̉���
    for (int j = 0; j < IMAGE_NUM; j++) {
        cv::projectPoints(obj_points[j], rvecs[j], tvecs[j], intrinsic, distortion, img_points[j]);
        for (const auto& point : img_points[j]) {
            cv::circle(srcImages[j], point, 5, cv::Scalar(0, 255, 0), -1); // �~��`��
        }
        // ���ʂ̕\��
        cv::imshow("Reprojection Result", srcImages[j]);
        cv::waitKey(0);
    }

    cv::FileStorage fs("camera.xml", cv::FileStorage::WRITE);
    if (!fs.isOpened())
    {
        cerr << "File can not be opened." << endl;
        return -1;
    }



    fs << "intrinsic" << intrinsic;
    fs << "distortion" << distortion;
    fs << "rotation" << rvecs;
    fs << "translation" << tvecs;
    fs << "rotation" << rvecs;
    fs << "transration" << tvecs;
    fs.release();

    return 0;
}
