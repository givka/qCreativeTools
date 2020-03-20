#include "tree_view.h"
#include "../utility.h"
#include <QDebug>

TreeView::TreeView()
        : QTreeView(),
          model(new QFileSystemModel)
{
    model->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    model->setRootPath(Utility::path + "/resources/");

    /* TODO: try to remove collapse icon for filtered dirs
    connect(this, &QTreeView::expanded, this,
            [](const QModelIndex &index) { qDebug() << "expanded" << index; });
    connect(this, &QTreeView::collapsed, this,
            [](const QModelIndex &index) { qDebug() << "collapsed" << index; });
    */
}

void TreeView::init()
{
    setModel(model);
    setRootIndex(model->index(model->rootPath()));

    // show only folder name
    hideColumn(1);
    hideColumn(2);
    hideColumn(3);

    qDebug() << currentIndex();
    expand(model->index("C:/dev/qCreativeTools/resources/data/"));

    // for (int i = 0; i < 3; ++i)
    //   resizeColumnToContents(i);
}

void TreeView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    emit selectedChanged(current, previous);
    QTreeView::currentChanged(current, previous);
}

void TreeView::updateTree(const QString &filePath)
{
    qDebug() << "TreeView::updateTree" << filePath;
    expand(model->index(filePath));
    setCurrentIndex(model->index(filePath));
}

