#include "icon_proxy.h"
#include "icon_provider.h"
#include <QtMultimedia/QMediaPlaylist>

IconProxy::IconProxy(QObject *parent)
        : QSortFilterProxyModel{ parent }
{
    connect(this, &IconProxy::hasIcon, this, &IconProxy::onIcon);
}

QVariant IconProxy::data(const QModelIndex &index, int role) const
{
    if (role == QFileSystemModel::FileIconRole) {
        auto path = index.data(QFileSystemModel::FilePathRole).toString();
        auto it = icons.find(path);
        if (it != icons.end()) {
            if (!it->isNull()) return *it;
            return QSortFilterProxyModel::data(index, role);
        }
        QPersistentModelIndex pIndex{ index };
        QtConcurrent::run([this, path, pIndex] {
            emit hasIcon(path, getIcon(path), pIndex);
        });
        return QVariant{};
    }
    return QSortFilterProxyModel::data(index, role);
}

void IconProxy::onIcon(const QString &path, const QIcon &icon, const QPersistentModelIndex &index)
{
    icons.insert(path, icon);
    emit dataChanged(index, index, QVector<int>{ QFileSystemModel::FileIconRole });
}

QIcon IconProxy::getIcon(const QString &path) const
{
    auto info = QFileInfo(path);
    auto filePath = info.filePath();
    auto suffix = info.suffix().toLower();
    const int Size = 128;

    if (IconProvider::ImageTypes.contains(suffix)) {
        auto pixMap = QPixmap();
        pixMap.load(filePath);

        // first scaled in fast transform for speed.
        pixMap = pixMap.scaled(2 * Size, 2 * Size, Qt::KeepAspectRatio);
        pixMap = pixMap.scaled(Size, Size, Qt::KeepAspectRatio,
                               Qt::TransformationMode::SmoothTransformation);

        return QIcon(pixMap);
    }
    return QIcon();
}

bool IconProxy::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{

    auto model = qobject_cast<QFileSystemModel *>(sourceModel());

    if (sourceParent == model->index(model->rootPath())) {
        return QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);
    }

    auto modelIndex = model->index(sourceRow, 0, sourceParent);

    auto info = model->fileInfo(modelIndex);
    auto suffix = info.suffix().toLower();

    qDebug() << info.fileName() << suffix << filters.contains(suffix);

    if (filters.isEmpty()) {
        return true;
    }



    /*if (info.isDir())
        return false;
*/
    return filters.contains(suffix);
}

void IconProxy::setFilters(const QStringList &list)
{
    filters = list;
}
