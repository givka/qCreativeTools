#pragma once

#include <QListView>
#include <QFileSystemModel>
#include <QtCore/QSortFilterProxyModel>
#include "icon_proxy.h"
#include "filter_model.h"

class ListView : public QListView
{
Q_OBJECT

public:
    ListView();

    void init();

    void setFilters(const QStringList &list);

signals:

    void showPreview(const QString &string);

    void updateTree(const QString &filePath);

public slots:

    void navigateTo(const QModelIndex &index);

private:

    void currentChanged(const QModelIndex &current, const QModelIndex &previous) override;

private:
    QFileSystemModel *model;
    IconProxy *iconProxy;
    FilterModel *filterModel;
};


