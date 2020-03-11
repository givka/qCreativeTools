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

public slots:

    void updateTree(const QString &filePath);

private slots:

    void currentChanged(const QModelIndex &current, const QModelIndex &previous) override;

private :
    QFileSystemModel *model;
};


