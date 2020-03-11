#pragma once

#include <QTreeView>
#include <QFileSystemModel>

class TreeView : public QTreeView
{
Q_OBJECT

public:
    TreeView();

    void init();

signals :

    void selectedChanged(const QModelIndex &current, const QModelIndex &previous);

private slots:

    void test(const QModelIndex &index);

    void currentChanged(const QModelIndex &current, const QModelIndex &previous) override;

private :
    QFileSystemModel *model;
};


