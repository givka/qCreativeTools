#pragma once

#include <QListView>
#include <QFileSystemModel>
#include <QtCore/QSortFilterProxyModel>

class ListView : public QListView
{
Q_OBJECT

public:
    ListView();

signals:

    void showPreview(const QString &string);

private:

    void currentChanged(const QModelIndex &current, const QModelIndex &previous) override;
};


