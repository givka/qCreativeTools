#pragma once

#include <QListView>
#include <QFileSystemModel>
#include <QtCore/QSortFilterProxyModel>

class ListView : public QListView
{
Q_OBJECT

public:
    ListView();

    void init();

    QFileSystemModel *getModel() { return model; };

signals:

    void showPreview(const QString &string);

    void updateTree(const QString &filePath);

private:
    void navigateTo(const QModelIndex &index);

    void currentChanged(const QModelIndex &current, const QModelIndex &previous) override;

private:
    QFileSystemModel *model;
};


