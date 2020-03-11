#include <QHBoxLayout>
#include <QListView>
#include "main_window.h"
#include "icon_provider.h"
#include "visual_preview/visual_preview.h"
#include "scene.h"
#include "inspector.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QtWidgets/QGroupBox>

MainWindow::MainWindow()
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

    auto groupBox10 = new QGroupBox("Preview");
    auto scene = new Scene;
    vBox1 = new QVBoxLayout;
    vBox1->addWidget(scene);
    groupBox10->setLayout(vBox1);

    auto gridLayout = new QGridLayout;
    gridLayout->addWidget(groupBox10, 0, 0, 2, 3);
    gridLayout->addWidget(groupBox1, 2, 0, 2, 1);
    gridLayout->addWidget(groupBox20, 2, 1, 2, 2);
    gridLayout->addWidget(groupBox2, 0, 3, 4, 1);

    // to keep the rows and columns the same size.
    for (int i = 0; i < gridLayout->rowCount(); i++)
        gridLayout->setRowStretch(i, 1);
    for (int i = 0; i < gridLayout->columnCount(); i++)
        gridLayout->setColumnStretch(i, 1);

    connect(treeView, &TreeView::selectedChanged, visualPreview, &VisualPreview::selectedChanged);
    connect(visualPreview->getListView(), &ListView::showPreview, scene, &Scene::setImage);
    connect(visualPreview->getListView(), &ListView::showPreview, inspector, &Inspector::setImage);

    window->setLayout(gridLayout);

    setCentralWidget(window);
    resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    qDebug() << "resize\n";

    QWidget::resizeEvent(event);
}
