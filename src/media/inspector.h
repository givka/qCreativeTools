#pragma once

#include <QWidget>
#include <QtMultimedia/QMediaPlayer>
#include "inspector_line.h"

class Inspector : public QWidget
{
Q_OBJECT

public:
    explicit Inspector(QWidget *parent = nullptr);

public slots:

    void setImage(const QString &path);

private:
    QMediaPlayer *m_mediaPlayer;

    QVector<InspectorLine *> lines;
};


