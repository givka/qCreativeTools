#pragma once

#include <QWidget>
#include <QtMultimedia/QMediaPlayer>

class Inspector : public QWidget
{
Q_OBJECT

public:
    explicit Inspector(QWidget *parent = nullptr);

public slots:

    void setImage(const QString &path);

private slots:

    void mediaStatusChanged(QMediaPlayer::MediaStatus status);

private:
    QMediaPlayer *m_mediaPlayer;
};


