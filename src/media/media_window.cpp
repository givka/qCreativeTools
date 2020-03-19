#include <QHBoxLayout>
#include <QListView>
#include "media_window.h"
#include "icon_provider.h"
#include "visual_preview.h"
#include "scene.h"
#include "inspector.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QtWidgets/QGroupBox>

MediaWindow::MediaWindow() : QMainWindow()
{
    auto window = new QWidget();

    treeView = new TreeView;
    treeView->init();

    auto groupBox1 = new QGroupBox("Folder Structure");
    auto vBox1 = new QVBoxLayout;
    vBox1->addWidget(treeView);
    groupBox1->setLayout(vBox1);

    auto visualPreview = new VisualPreview;

    auto groupBox20 = new QGroupBox("Icon Preview");
    vBox1 = new QVBoxLayout;
    vBox1->addWidget(visualPreview);
    groupBox20->setLayout(vBox1);

    auto inspector = new Inspector;

    auto groupBox2 = new QGroupBox("Inspector");
    vBox1 = new QVBoxLayout;
    vBox1->addWidget(inspector);
    groupBox2->setLayout(vBox1);

    auto groupBox10 = new QGroupBox("Scene");
    auto scene = new Scene;
    vBox1 = new QVBoxLayout;
    vBox1->addWidget(scene);
    groupBox10->setLayout(vBox1);

    auto gridLayout = new QGridLayout;
    gridLayout->addWidget(groupBox10, 0, 0, 3, 3);
    gridLayout->addWidget(groupBox1, 3, 0, 2, 1);
    gridLayout->addWidget(groupBox20, 3, 1, 2, 2);
    gridLayout->addWidget(groupBox2, 0, 3, 5, 1);


    // to keep the rows and columns the same size.
    for (int i = 0; i < gridLayout->rowCount(); i++)
        gridLayout->setRowStretch(i, 1);
    for (int i = 0; i < gridLayout->columnCount(); i++)
        gridLayout->setColumnStretch(i, 1);

    connect(treeView, &TreeView::selectedChanged, visualPreview->getListView(),
            &ListView::navigateTo);
    connect(visualPreview->getListView(), &ListView::showPreview, scene, &Scene::setImage);
    connect(visualPreview->getListView(), &ListView::showPreview, inspector, &Inspector::setImage);
    connect(visualPreview->getListView(), &ListView::updateTree, treeView, &TreeView::updateTree);

    window->setLayout(gridLayout);

    setCentralWidget(window);
}

void MediaWindow::resizeEvent(QResizeEvent *event)
{
    qDebug() << "resize\n";

    QWidget::resizeEvent(event);
}
