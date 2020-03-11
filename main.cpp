#include <QApplication>
#include <QPushButton>
#include "src/main_window.h"


int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    return QApplication::exec();
}