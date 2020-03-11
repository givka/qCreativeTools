#include "icon_provider.h"
#include <QDebug>

const QStringList IconProvider::ImageTypes = { "bmp", "gif", "jpg", "jpeg", "png", "pbm", "pgm",
                                               "ppm", "xbm", "xpm" };
const QStringList IconProvider::VideoTypes = { "mp4", "avi" };

QIcon IconProvider::icon(const QFileInfo &info) const
{
    auto filePath = info.filePath();

    if (ImageTypes.contains(info.suffix().toLower())) {
        auto pixMap = QPixmap();
        pixMap.load(filePath);
        pixMap = pixMap.scaled(Size, Size, Qt::KeepAspectRatio,
                               Qt::TransformationMode::SmoothTransformation);
        return QIcon(pixMap);
    }

    auto pixMap = QFileIconProvider::icon(info).pixmap(QSize(Size, Size));

    pixMap = pixMap.scaled(Size, Size, Qt::KeepAspectRatio,
                           Qt::TransformationMode::SmoothTransformation);

    return QIcon(pixMap);
}

