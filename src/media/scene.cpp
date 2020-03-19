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
#include <QTabWidget>
#include <QSplitter>

Scene::Scene(QWidget *parent)
        : QWidget(parent),
          imageLabel(new QLabel),
          lHist(new QLabel), sHist(new QLabel), rHist(new QLabel), gHist(new QLabel),
          bHist(new QLabel),
          lWaveform(new QLabel), sWaveform(new QLabel), rWaveform(new QLabel),
          gWaveform(new QLabel), bWaveform(new QLabel),
          vectorscope(new QLabel)
{
    auto histogramsTab = new QTabWidget;

    auto w = new QWidget;
    auto l = new QVBoxLayout;
    l->addWidget(lHist);
    w->setLayout(l);
    histogramsTab->addTab(w, "Luminance");
    w = new QWidget;
    l = new QVBoxLayout;
    l->addWidget(sHist);
    w->setLayout(l);
    histogramsTab->addTab(w, "RGB");
    w = new QWidget;
    l = new QVBoxLayout;
    l->addWidget(rHist);
    w->setLayout(l);
    histogramsTab->addTab(w, "R");
    w = new QWidget;
    l = new QVBoxLayout;
    l->addWidget(gHist);
    w->setLayout(l);
    histogramsTab->addTab(w, "G");
    w = new QWidget;
    l = new QVBoxLayout;
    l->addWidget(bHist);
    w->setLayout(l);
    histogramsTab->addTab(w, "B");

    auto histograms = new QGroupBox("Histograms");
    auto lay = new QVBoxLayout;
    lay->addWidget(histogramsTab);
    histograms->setLayout(lay);

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

    auto waveformsTab = new QTabWidget;
    w = new QWidget;
    l = new QVBoxLayout;
    l->addWidget(lWaveform);
    w->setLayout(l);
    waveformsTab->addTab(w, "Luminance");
    w = new QWidget;
    l = new QVBoxLayout;
    l->addWidget(sWaveform);
    w->setLayout(l);
    waveformsTab->addTab(w, "RGB");
    w = new QWidget;
    l = new QVBoxLayout;
    l->addWidget(rWaveform);
    w->setLayout(l);
    waveformsTab->addTab(w, "R");
    w = new QWidget;
    l = new QVBoxLayout;
    l->addWidget(gWaveform);
    w->setLayout(l);
    waveformsTab->addTab(w, "G");
    w = new QWidget;
    l = new QVBoxLayout;
    l->addWidget(bWaveform);
    w->setLayout(l);
    waveformsTab->addTab(w, "B");

    auto waveforms = new QGroupBox("Waveforms");
    lay = new QVBoxLayout;
    lay->addWidget(waveformsTab);
    waveforms->setLayout(lay);

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

    auto preview = new QGroupBox("Preview");
    lay = new QVBoxLayout;
    lay->addWidget(imageLabel);
    preview->setLayout(lay);

    auto vectorScope = new QGroupBox("Vectorscope");
    lay = new QVBoxLayout;
    lay->addWidget(vectorscope);
    vectorScope->setLayout(lay);

/*
    auto hLayout = new QHBoxLayout;
     hLayout->addWidget(preview);
     hLayout->addWidget(histograms);
     hLayout->addWidget(waveforms);
     hLayout->addWidget(vectorScope);
     setLayout(hLayout);

     hLayout->setStretch(0, 1);
     hLayout->setStretch(1, 1);
     hLayout->setStretch(2, 1);
     hLayout->setStretch(3, 1);

*/

    auto gridLayout = new QGridLayout;
    gridLayout->addWidget(preview, 0, 0, 1, 3);
    gridLayout->addWidget(histograms, 1, 0);
    gridLayout->addWidget(waveforms, 1, 1);
    gridLayout->addWidget(vectorScope, 1, 2);

    // to keep the rows and columns the same size.
    for (int i = 0; i < gridLayout->rowCount(); i++)
        gridLayout->setRowStretch(i, 1);
    for (int i = 0; i < gridLayout->columnCount(); i++)
        gridLayout->setColumnStretch(i, 1);

    setLayout(gridLayout);

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
        vectorscope->setPixmap(QPixmap());
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
    int factor = 50;
    auto lWaveformMat = OpenCV::calcWaveform(luminance, cv::Scalar(factor, factor, factor, 255));
    auto rWaveformMat = OpenCV::calcWaveform(channels[2], cv::Scalar(0, 0, factor, 255));
    auto gWaveformMat = OpenCV::calcWaveform(channels[1], cv::Scalar(0, factor, 0, 255));
    auto bWaveformMat = OpenCV::calcWaveform(channels[0], cv::Scalar(factor, 0, 0, 255));
    auto sWaveformMat = (rWaveformMat + gWaveformMat + bWaveformMat);

    lWaveform->setPixmap(OpenCV::matToPixmap(lWaveformMat, w, h));
    sWaveform->setPixmap(OpenCV::matToPixmap(sWaveformMat, w, h));
    rWaveform->setPixmap(OpenCV::matToPixmap(rWaveformMat, w, h));
    gWaveform->setPixmap(OpenCV::matToPixmap(gWaveformMat, w, h));
    bWaveform->setPixmap(OpenCV::matToPixmap(bWaveformMat, w, h));

    w = vectorscope->width();
    h = vectorscope->height();

    auto vectorscopeMat = OpenCV::calcVectorscope(src);
    vectorscope->setPixmap(OpenCV::matToPixmap(vectorscopeMat, w, h));
}





