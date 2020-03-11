#include "tree_view.h"
#include <QDebug>

TreeView::TreeView()
        : QTreeView(), model(new QFileSystemModel)
{
    connect(this, &QTreeView::expanded, this, &TreeView::test);
    connect(this, &QTreeView::collapsed, this, &TreeView::test);



    model->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    model->setRootPath("C:/dev/myman/resources/");



}

void TreeView::init()
{
    setModel(model);
    setRootIndex(model->index(model->rootPath()));

    // show only folder name
    hideColumn(1);
    hideColumn(2);
    hideColumn(3);

    // for (int i = 0; i < 3; ++i)
    //   resizeColumnToContents(i);

}

void TreeView::test(const QModelIndex &index)
{
}

void TreeView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
   resizeColumnToContents(0);

    emit selectedChanged(current, previous);
    QTreeView::currentChanged(current, previous);
}
