#include "scene.h"
#include "icon_provider.h"
#include <QDebug>
#include <QtWidgets/QVBoxLayout>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <QGroupBox>
#include <opencv2/highgui.hpp>
#include <QFileInfo>

Scene::Scene(QWidget *parent)
        : QWidget(parent),
          imageLabel(new QLabel),
          lHist(new QLabel), sHist(new QLabel), rHist(new QLabel), gHist(new QLabel),
          bHist(new QLabel)
{

    auto histograms = new QGroupBox("Histograms");
    auto vLayout = new QVBoxLayout;
    vLayout->addWidget(lHist);
    vLayout->addWidget(sHist);
    vLayout->addWidget(rHist);
    vLayout->addWidget(gHist);
    vLayout->addWidget(bHist);
    histograms->setLayout(vLayout);

    lHist->setBackgroundRole(QPalette::Base);
    lHist->setAutoFillBackground(true);
    sHist->setBackgroundRole(QPalette::Base);
    sHist->setAutoFillBackground(true);
    rHist->setBackgroundRole(QPalette::Base);
    rHist->setAutoFillBackground(true);
    gHist->setBackgroundRole(QPalette::Base);
    gHist->setAutoFillBackground(true);
    bHist->setBackgroundRole(QPalette::Base);
    bHist->setAutoFillBackground(true);

    auto hLayout = new QHBoxLayout;
    hLayout->addWidget(imageLabel);
    hLayout->addWidget(histograms);
    setLayout(hLayout);

    imageLabel->setAlignment(Qt::AlignCenter);
}

void Scene::setImage(const QString &path)
{
    if (path.isEmpty()) {
        imageLabel->setPixmap(QPixmap());
        lHist->setPixmap(QPixmap());
        sHist->setPixmap(QPixmap());
        rHist->setPixmap(QPixmap());
        gHist->setPixmap(QPixmap());
        bHist->setPixmap(QPixmap());
        return;
    }

    auto pixMap = QPixmap(path);
    int w = imageLabel->width();
    int h = imageLabel->height();

    //.scaled(2*w, 2*h).scaled(w, h, Qt::TransformationMode::SmoothTransformation) is way faster
    imageLabel->setPixmap(
            pixMap.scaled(w, h, Qt::KeepAspectRatio, Qt::TransformationMode::SmoothTransformation));

    auto suffix = QFileInfo(path).suffix().toLower();
    if (suffix == "gif" || IconProvider::VideoTypes.contains(suffix))
        return;

    cv::Mat src = cv::imread(path.toStdString(), 1);
    std::vector<cv::Mat> channels{};
    split(src, channels);

    cv::Mat luminance;
    cv::cvtColor(src, luminance, CV_BGR2GRAY);

    w = lHist->width();
    h = lHist->height();

    // careful, opencv in BGR format.
    auto lMat = calculateHistogram(luminance, cv::Scalar(255, 255, 255, 255));
    auto rMat = calculateHistogram(channels[2], cv::Scalar(0, 0, 255, 255));
    auto gMat = calculateHistogram(channels[1], cv::Scalar(0, 255, 0, 255));
    auto bMat = calculateHistogram(channels[0], cv::Scalar(255, 0, 0, 255));
    auto sMat = (rMat + gMat + bMat);

    lHist->setPixmap(matToPixmap(lMat, w, h));
    sHist->setPixmap(matToPixmap(sMat, w, h));
    rHist->setPixmap(matToPixmap(rMat, w, h));
    gHist->setPixmap(matToPixmap(gMat, w, h));
    bHist->setPixmap(matToPixmap(bMat, w, h));
}

cv::Mat Scene::calculateHistogram(const cv::Mat &channel, const cv::Scalar &bgraColor)
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
    calcHist(&channel, 1, nullptr, cv::Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);
    cv::Mat histImage(hist_h, hist_w, CV_8UC4, cv::Scalar(0, 0, 0, 0));

    normalize(hist, hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

    for (int i = 1; i < histSize; i++) {
        cv::line(histImage,
                 cv::Point(bin_w * (i - 1) - thickness, hist_h - cvRound(hist.at<float>(i - 1))),
                 cv::Point(bin_w * (i - 1) - thickness, hist_h),
                 bgraColor, thickness, CV_AA, 0);
    }

    return histImage.clone();
}

QPixmap Scene::matToPixmap(const cv::Mat &mat, int width, int height)
{
    // strange that it works with a const reference
    cv::cvtColor(mat, mat, CV_BGRA2RGBA);
    auto image = QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGBA8888);
    return QPixmap::fromImage(image).scaled(width, height,
                                            Qt::IgnoreAspectRatio,
                                            Qt::TransformationMode::SmoothTransformation);
}

