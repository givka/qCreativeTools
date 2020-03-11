#include "tree_view.h"
#include <QDebug>

TreeView::TreeView()
        : QTreeView(),
          model(new QFileSystemModel)
{
    model->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    model->setRootPath("C:/dev/qCreativeTools/resources/");
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

void TreeView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    emit selectedChanged(current, previous);
    QTreeView::currentChanged(current, previous);
}
