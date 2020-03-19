#pragma once

#include <QMainWindow>
#include <QTreeView>
#include "tree_view.h"
#include "list_view.h"

class MediaWindow : public QMainWindow
{
Q_OBJECT

public:

    MediaWindow();

    void resizeEvent(QResizeEvent *event) override;

private:

    TreeView *treeView;
};


