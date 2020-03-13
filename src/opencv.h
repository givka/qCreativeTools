//
// Created by Arthur Barriere on 13/03/2020.
//

#ifndef QCREATIVETOOLS_OPENCV_H
#define QCREATIVETOOLS_OPENCV_H

#include <opencv2/core/mat.hpp>
#include <QPixmap>

class OpenCV
{
public:
    static cv::Mat calcHistogram(const cv::Mat &channel, const cv::Vec4b &bgraColor);

    static cv::Mat calcWaveform(const cv::Mat &src, const cv::Vec4b &color);

    static QPixmap matToPixmap(const cv::Mat &src, int width, int height);
};

#endif //QCREATIVETOOLS_OPENCV_H
