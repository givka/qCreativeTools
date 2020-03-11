#include "inspector.h"
#include <QMediaMetaData>
#include <QtMultimedia/QMediaPlayer>

Inspector::Inspector(QWidget *parent) :
        QWidget(parent),
        m_mediaPlayer(new QMediaPlayer)
{
    connect(m_mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &Inspector::mediaStatusChanged);
}

void Inspector::setImage(const QString &path)
{
    qDebug() << "Inspector::setImage" << path;

    m_mediaPlayer->setMedia(QUrl::fromLocalFile(path));
}

//TODO: test exif from github
void Inspector::mediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    qDebug() << "mediaStatusChanged" << status;
    QStringList metadatalist = m_mediaPlayer->availableMetaData();

    int list_size = metadatalist.size();

    qDebug() << m_mediaPlayer->isMetaDataAvailable() << list_size;

    QString metadata_key;
    QVariant var_data;

    for (int indx = 0; indx < list_size; indx++) {
        // Get the key from the list
        metadata_key = metadatalist.at(indx);

        // Get the value for the key
        var_data = m_mediaPlayer->metaData(metadata_key);

        qDebug() << metadata_key << var_data.toString();
    }
}



