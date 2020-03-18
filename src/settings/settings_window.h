#pragma once

#include <QMainWindow>
#include "draw_area.h"
#include "settings.h"

class SettingsWindow : public QMainWindow
{
public:
    SettingsWindow();

    DrawArea *drawArea;
    Settings *settings;
};
