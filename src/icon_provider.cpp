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

    if (IconProvider::ImageTypes.contains(suffix)) {
        auto pixMap = QPixmap();
        pixMap.load(filePath);

        // first scaled in fast transform for speed.
        pixMap = pixMap.scaled(2 * Size, 2 * Size, Qt::KeepAspectRatio);
        pixMap = pixMap.scaled(Size, Size, Qt::KeepAspectRatio,
                               Qt::TransformationMode::SmoothTransformation);

        return QIcon(pixMap);
    }

    auto pixMap = QFileIconProvider::icon(info).pixmap(QSize(Size, Size));

    pixMap = pixMap.scaled(Size, Size, Qt::KeepAspectRatio,
                           Qt::TransformationMode::SmoothTransformation);

    return QIcon(pixMap);
}

