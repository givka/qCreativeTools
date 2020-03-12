#include <QtWidgets/QFileSystemModel>
#include "filter_model.h"

FilterModel::FilterModel(QObject *parent)
        : QSortFilterProxyModel()
{
}

bool FilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{

    return QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);
    if (!sourceParent.isValid())
        return false;

    auto model = qobject_cast<QFileSystemModel *>(sourceModel());

    auto modelIndex = model->index(sourceRow, 0, sourceParent);

    auto info = model->fileInfo(modelIndex);
    qDebug() << sourceRow << info.filePath();

    return true;

    auto suffix = info.suffix().toLower();
    return filters.empty() || filters.contains(suffix);
}