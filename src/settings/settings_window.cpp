#include <QHBoxLayout>
#include <QGraphicsView>
#include <QDebug>
#include <QGroupBox>
#include "settings_window.h"
#include "settings.h"

SettingsWindow::SettingsWindow() :
        QMainWindow(), settings(new Settings)
{
    auto scene = settings->getScene();
    auto view = new QGraphicsView(scene);

    view->setSceneRect(view->rect());
    view->setRenderHint(QPainter::Antialiasing, true);

    auto layout = new QHBoxLayout();

    auto groupBox = new QGroupBox("Draw Area");
    auto hLayout = new QHBoxLayout;
    hLayout->addWidget(view);
    groupBox->setLayout(hLayout);

    layout->addWidget(groupBox);

    groupBox = new QGroupBox("Settings");
    hLayout = new QHBoxLayout;
    hLayout->addWidget(settings);
    groupBox->setLayout(hLayout);

    layout->addWidget(groupBox);

    layout->setStretch(0, 2);
    layout->setStretch(1, 1);

    auto window = new QWidget;
    window->setLayout(layout);

    setCentralWidget(window);
}
