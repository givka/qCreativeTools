#include <QHBoxLayout>
#include <QGroupBox>
#include "settings_window.h"
#include "draw_area.h"
#include "settings.h"

SettingsWindow::SettingsWindow() :
        QMainWindow(),
        drawArea(new DrawArea), settings(new Settings)
{
    auto layout = new QHBoxLayout();

    auto groupBox = new QGroupBox("Draw Area");
    auto hLayout = new QHBoxLayout;
    hLayout->addWidget(drawArea);
    groupBox->setLayout(hLayout);

    layout->addWidget(groupBox);

    groupBox = new QGroupBox("Settings");
    hLayout = new QHBoxLayout;
    hLayout->addWidget(settings);
    groupBox->setLayout(hLayout);

    layout->addWidget(groupBox);

    layout->setStretch(0, 3);
    layout->setStretch(1, 1);


    auto window = new QWidget;
    window->setLayout(layout);



    setCentralWidget(window);
}
