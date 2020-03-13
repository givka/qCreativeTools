#pragma once

#include <QWidget>
#include <QLabel>
#include <opencv2/core/mat.hpp>

class Scene : public QWidget
{
Q_OBJECT

public :

    explicit Scene(QWidget *parent = nullptr);

public slots:

    void setImage(const QString &path);

private :
    QLabel *imageLabel;

    static cv::Mat *calculateHistogram(const cv::Mat &channel, const cv::Scalar &bgraColor);

    static QPixmap matToPixmap(const cv::Mat &mat, int width, int height);

    QLabel *lHist;
    QLabel *rHist;
    QLabel *bHist;
    QLabel *gHist;

    cv::Mat *lMat;
    cv::Mat *rMat;
    cv::Mat *gMat;
    cv::Mat *bMat;
};


