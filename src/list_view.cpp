#include "list_view.h"
#include "icon_provider.h"
#include <QDebug>

ListView::ListView() : QListView()
{
    connect(this, &QAbstractItemView::doubleClicked, this, &ListView::navigateTo);
}

void ListView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    auto info = qobject_cast<QFileSystemModel *>(model())->fileInfo(current);
    auto suffix = info.suffix().toLower();

    if (IconProvider::ImageTypes.contains(suffix) || IconProvider::VideoTypes.contains(suffix)) {
        qDebug() << "emit showPreview";
        emit showPreview(info.filePath());
    }

    QListView::currentChanged(current, previous);
}

void ListView::navigateTo(const QModelIndex &index)
{
    auto model = qobject_cast<QFileSystemModel *>(this->model());
    auto info = model->fileInfo(index);

    if (info.isDir()) {
        setRootIndex(model->index(info.filePath()));
        emit updateTree(info.filePath());
    }
}
