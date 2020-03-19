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

    QLabel *lHist;
    QLabel *sHist;
    QLabel *rHist;
    QLabel *bHist;
    QLabel *gHist;

    QLabel *lWaveform;
    QLabel *sWaveform;
    QLabel *rWaveform;
    QLabel *bWaveform;
    QLabel *gWaveform;

    QLabel *vectorscope;
};


