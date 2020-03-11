#include "icon_provider.h"
#include <QDebug>
#include <QMovie>

const QStringList IconProvider::ImageTypes = { "bmp", "cur", "gif", "icns", "ico", "jpeg", "jpg",
                                               "pbm", "pgm", "png", "ppm", "svg", "svgz",
                                               "tga", "tif", "tiff", "wbmp", "webp", "xbm", "xpm" };
const QStringList IconProvider::VideoTypes = { "mp4", "avi" };

QIcon IconProvider::icon(const QFileInfo &info) const
{

    auto filePath = info.filePath();
    auto suffix = info.suffix().toLower();

    if (ImageTypes.contains(suffix)) {
        auto pixMap = QPixmap();
        pixMap.load(filePath);
        pixMap = pixMap.scaled(Size, Size, Qt::KeepAspectRatio,
                               Qt::TransformationMode::FastTransformation);
        return QIcon(pixMap);
    }

    if (VideoTypes.contains(suffix)) {

        auto movie = new QMovie(filePath);
        movie->start();

        qDebug() << movie->state();

        auto pixMap = movie->currentPixmap();
        pixMap = pixMap.scaled(Size, Size, Qt::KeepAspectRatio,
                               Qt::TransformationMode::FastTransformation);
        return QIcon(pixMap);
    }

    auto pixMap = QFileIconProvider::icon(info).pixmap(QSize(Size, Size));

    pixMap = pixMap.scaled(Size, Size, Qt::KeepAspectRatio,
                           Qt::TransformationMode::SmoothTransformation);

    return QIcon(pixMap);
}

