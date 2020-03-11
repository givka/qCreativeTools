#pragma once

#include <QMainWindow>
#include <QTreeView>
#include "tree_view.h"
#include "list_view.h"

class MainWindow : public QMainWindow
{
Q_OBJECT

public:

    MainWindow();

    void resizeEvent(QResizeEvent *event) override;

private:

    TreeView *treeView;
};


