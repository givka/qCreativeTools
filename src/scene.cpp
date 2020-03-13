#include "scene.h"
#include "icon_provider.h"
#include "opencv.h"
#include <QDebug>
#include <QtWidgets/QVBoxLayout>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <QGroupBox>
#include <QFileInfo>

Scene::Scene(QWidget *parent)
        : QWidget(parent),
          imageLabel(new QLabel),
          lHist(new QLabel), sHist(new QLabel), rHist(new QLabel), gHist(new QLabel),
          bHist(new QLabel),
          lWaveform(new QLabel), sWaveform(new QLabel), rWaveform(new QLabel),
          gWaveform(new QLabel), bWaveform(new QLabel)
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

    auto waveforms = new QGroupBox("Waveforms");
    auto layout = new QGridLayout;
    layout->addWidget(lWaveform, 0, 0);
    layout->addWidget(sWaveform, 0,1);
    layout->addWidget(rWaveform,1,0);
    layout->addWidget(gWaveform,1,1);
    layout->addWidget(bWaveform,1,2);
    waveforms->setLayout(layout);

    lWaveform->setBackgroundRole(QPalette::Base);
    lWaveform->setAutoFillBackground(true);
    sWaveform->setBackgroundRole(QPalette::Base);
    sWaveform->setAutoFillBackground(true);
    rWaveform->setBackgroundRole(QPalette::Base);
    rWaveform->setAutoFillBackground(true);
    gWaveform->setBackgroundRole(QPalette::Base);
    gWaveform->setAutoFillBackground(true);
    bWaveform->setBackgroundRole(QPalette::Base);
    bWaveform->setAutoFillBackground(true);

    auto hLayout = new QHBoxLayout;
    hLayout->addWidget(imageLabel);
    hLayout->addWidget(histograms);
    hLayout->addWidget(waveforms);
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
        lWaveform->setPixmap(QPixmap());
        sWaveform->setPixmap(QPixmap());
        rWaveform->setPixmap(QPixmap());
        gWaveform->setPixmap(QPixmap());
        bWaveform->setPixmap(QPixmap());
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
    auto lHistMat = OpenCV::calcHistogram(luminance, cv::Scalar(255, 255, 255, 255));
    auto rHistMat = OpenCV::calcHistogram(channels[2], cv::Scalar(0, 0, 255, 255));
    auto gHistMat = OpenCV::calcHistogram(channels[1], cv::Scalar(0, 255, 0, 255));
    auto bHistMat = OpenCV::calcHistogram(channels[0], cv::Scalar(255, 0, 0, 255));
    auto sHistMat = (rHistMat + gHistMat + bHistMat);

    lHist->setPixmap(OpenCV::matToPixmap(lHistMat, w, h));
    sHist->setPixmap(OpenCV::matToPixmap(sHistMat, w, h));
    rHist->setPixmap(OpenCV::matToPixmap(rHistMat, w, h));
    gHist->setPixmap(OpenCV::matToPixmap(gHistMat, w, h));
    bHist->setPixmap(OpenCV::matToPixmap(bHistMat, w, h));

    w = lWaveform->width();
    h = lWaveform->height();

    // careful, opencv in BGR format.
    auto lWaveformMat = OpenCV::calcWaveform(luminance, cv::Scalar(255, 255, 255, 255));
    auto rWaveformMat = OpenCV::calcWaveform(channels[2], cv::Scalar(0, 0, 255, 255));
    auto gWaveformMat = OpenCV::calcWaveform(channels[1], cv::Scalar(0, 255, 0, 255));
    auto bWaveformMat = OpenCV::calcWaveform(channels[0], cv::Scalar(255, 0, 0, 255));
    auto sWaveformMat = (rWaveformMat + gWaveformMat + bWaveformMat);

    lWaveform->setPixmap(OpenCV::matToPixmap(lWaveformMat, w, h));
    sWaveform->setPixmap(OpenCV::matToPixmap(sWaveformMat, w, h));
    rWaveform->setPixmap(OpenCV::matToPixmap(rWaveformMat, w, h));
    gWaveform->setPixmap(OpenCV::matToPixmap(gWaveformMat, w, h));
    bWaveform->setPixmap(OpenCV::matToPixmap(bWaveformMat, w, h));
}





