//
// Created by Arthur Barriere on 13/03/2020.
//

#include "opencv.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

cv::Mat OpenCV::calcHistogram(const cv::Mat &channel, const cv::Vec4b &bgraColor)
{
    int scale = 4;
    int hist_w = scale * 256;
    int hist_h = (int) ((float) hist_w * 9 / 16);

    int histSize = 256;
    int bin_w = hist_w / histSize;
    int thickness = (scale + 1) / 2;
    bool uniform = true;
    bool accumulate = false;

    float range[] = { 0, 256 }; //the upper boundary is exclusive
    const float *histRange = { range };

    cv::Mat hist;
    cv::calcHist(&channel, 1, nullptr, cv::Mat(), hist, 1, &histSize, &histRange, uniform,
                 accumulate);
    cv::Mat histImage(hist_h, hist_w, CV_8UC4, cv::Scalar(0, 0, 0, 0));

    normalize(hist, hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

    for (int i = 1; i < histSize; i++) {
        cv::line(histImage,
                 cv::Point(bin_w * (i - 1) - thickness, hist_h - cvRound(hist.at<float>(i - 1))),
                 cv::Point(bin_w * (i - 1) - thickness, hist_h),
                 bgraColor, thickness, CV_AA, 0);
    }

    return histImage;
}

QPixmap OpenCV::matToPixmap(const cv::Mat &src, int width, int height)
{
    // we use ptr for QImage so we need a deep copy.
    auto mat = src.clone();
    cv::cvtColor(mat, mat, CV_BGRA2RGBA);
    auto image = QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGBA8888);
    return QPixmap::fromImage(image).scaled(width, height,
                                            Qt::IgnoreAspectRatio,
                                            Qt::TransformationMode::SmoothTransformation);
}

cv::Mat OpenCV::calcWaveform(const cv::Mat &src, const cv::Vec4b &color)
{
    int height = 200;
    cv::Mat scope = cv::Mat(height, src.cols, CV_8UC4, cv::Scalar(0, 0, 0, 0));

    for (int c = 0; c < src.cols; ++c) {
        for (int r = 0; r < src.rows; ++r) {
            float value = (float) src.at<uchar>(r, c) / 255.f;
            int row = height - (int) (value * (float) (height - 1)) - 1;
            scope.at<cv::Vec4b>(row, c) += color;
        }
    }
    return scope;
}

void calc(int &row, int &column, int size, const cv::Vec3b &value)
{

    int Y = value[0];
    int Cr = value[1];
    int Cb = value[2];

    float y = Y / 255.f;
    float cr = Cr / 255.f * 2 - 1;
    float cb = Cb / 255.f * 2 - 1;


    // hacked values to get into grid
    cr = -cr;

    row = size / 2 + y * cr * size / 2;
    column = size / 2 + y * cb * size / 2;

    // std::cout << row << ", " << column << "\n";
}

void calcRect(cv::Mat &mat, const cv::Scalar &color, float sizeRect)
{
    cv::Mat a;
    cv::Mat b(1, 1, CV_8UC3, color);
    cvtColor(b, a, CV_BGR2YCrCb);

    int r, c;
    cv::Vec3b value = cv::Vec3b(a.data[0], a.data[1], a.data[2]);
    calc(r, c, mat.rows, value);

    rectangle(mat,
              cv::Point(c - sizeRect, r - sizeRect),
              cv::Point(c + sizeRect, r + sizeRect), color, 1);
}

cv::Mat OpenCV::calcVectorscope(const cv::Mat &src)
{
    int size = 400;

    cv::Mat ycrcb;
    cvtColor(src, ycrcb, CV_BGR2YCrCb);

    //cv::Mat screen = cv::imread("../vectorscope.png", -1);
    //resize(screen, screen, cv::Size(size, size), 0, 0, cv::INTER_CUBIC);

    cv::Mat vectorScope = cv::Mat(size, size, CV_8UC4, cv::Vec4b(25, 25, 25, 255));

//    Mat perspective = getPerspectiveTransform(points1, points2);

    for (int row = 0; row < ycrcb.rows; ++row) {
        for (int column = 0; column < ycrcb.cols; ++column) {
            cv::Vec3b value = ycrcb.at<cv::Vec3b>(row, column);

            auto color = src.at<cv::Vec3b>(row, column);
            int r, c;
            calc(r, c, size, value);

            vectorScope.at<cv::Vec4b>(r, c) += 0.2 * cv::Vec4b(color[0], color[1],
                                                               color[2], 255);

            //circle(vectorScope, Point(c, r), 3, color, -1);
        }
    }


    /*
     for (int row = 0; row < vectorScope.rows; ++row) {
         for (int column = 0; column < vectorScope.cols; ++column) {
             auto value = ycrcb.at<Vec3b>(row, column);

             auto mask = screen.at<Vec4b>(row, column);
             auto color = 0.1 * value;
             //if(mask[3]>0.f)
             vectorScope.at<Vec4b>(row, column) += Vec4b(color[0], color[1], color[2], 255);
         }
     }
    */

    float sizeRect = 10;
    float sizeRect75 = 10;

    /*
    0.113725, -0.160784, 1
    0.588235, -0.835294, -0.662745
    0.298039, 1, -0.333333
    0.886275, 0.168628, -0.992157
    0.411765, 0.843137, 0.670588
    0.113725, -0.160784, 1
    0.701961, -1, 0.341177
    */

    /*
        203, 222
        298, 122
        140, 180
        170, 24
        130, 255
        203, 222
        340, 247
     *
     *
     *
     *
     *
     *
     */

    calcRect(vectorScope, cv::Scalar(0, 255, 0,255), sizeRect);
    calcRect(vectorScope, cv::Scalar(0, 0, 255,255), sizeRect);
    calcRect(vectorScope, cv::Scalar(0, 255, 255,255), sizeRect);
    calcRect(vectorScope, cv::Scalar(255, 0, 255,255), sizeRect);
    calcRect(vectorScope, cv::Scalar(255, 0, 0,255), sizeRect);
    calcRect(vectorScope, cv::Scalar(255, 255, 0,255), sizeRect);

    calcRect(vectorScope, 0.75f * cv::Scalar(255, 0, 0,255), sizeRect * 0.75f);
    calcRect(vectorScope, 0.75f * cv::Scalar(0, 255, 0,255), sizeRect * 0.75f);
    calcRect(vectorScope, 0.75f * cv::Scalar(0, 0, 255,255), sizeRect * 0.75f);
    calcRect(vectorScope, 0.75f * cv::Scalar(0, 255, 255,255), sizeRect * 0.75f);
    calcRect(vectorScope, 0.75f * cv::Scalar(255, 0, 255,255), sizeRect * 0.75f);
    calcRect(vectorScope, 0.75f * cv::Scalar(255, 0, 0,255), sizeRect * 0.75f);
    calcRect(vectorScope, 0.75f * cv::Scalar(255, 255, 0,255), sizeRect * 0.75f);

    return vectorScope;
}
