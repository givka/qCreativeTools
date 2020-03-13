//
// Created by Arthur Barriere on 13/03/2020.
//

#include "opencv.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

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
    int height = 100;
    cv::Mat scope = cv::Mat(height, src.cols, CV_8UC4, cv::Scalar(0, 0, 0, 0));

    for (int c = 0; c < src.cols; ++c) {
        for (int r = 0; r < src.rows; ++r) {
            float value = (float) src.at<uchar>(r, c) / 255.f;
            int row = height - (int) (value * (float) (height-1)) - 1;
            scope.at<cv::Vec4b>(row, c) += color;
        }
    }
    return scope;
}
