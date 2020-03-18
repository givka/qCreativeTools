#pragma once

#include <QMainWindow>
#include "settings.h"

class SettingsWindow : public QMainWindow
{
public:
    SettingsWindow();

    Settings *settings;
};
