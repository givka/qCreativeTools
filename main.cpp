#define THEME 1

#include <QApplication>
#include <QPushButton>
#include "src/main_window.h"
#include "src/settings_window.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

#if THEME == 1
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    QApplication::setPalette(darkPalette);

    app.setStyleSheet(
            "QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");

#elif THEME == 2
    app.setStyle(QStyleFactory::create("fusion"));

    QPalette palette;
    palette.setColor(QPalette::Window, QColor(53,53,53));
    palette.setColor(QPalette::WindowText, Qt::white);
    palette.setColor(QPalette::Base, QColor(15,15,15));
    palette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    palette.setColor(QPalette::ToolTipBase, Qt::white);
    palette.setColor(QPalette::ToolTipText, Qt::white);
    palette.setColor(QPalette::Text, Qt::white);
    palette.setColor(QPalette::Button, QColor(53,53,53));
    palette.setColor(QPalette::ButtonText, Qt::white);
    palette.setColor(QPalette::BrightText, Qt::red);

    palette.setColor(QPalette::Highlight, QColor(142,45,197).lighter());
    palette.setColor(QPalette::HighlightedText, Qt::black);
    QApplication::setPalette(palette);
#endif

#if __APPLE__
    QApplication::setWindowIcon(QIcon("/Users/dev/qCreativeTools/resources/icon.png"));
#elif _WIN32
    QApplication::setWindowIcon(QIcon("C:/dev/qCreativeTools/resources/icon.png"));
#endif

/*
    MainWindow mainWindow;
    mainWindow.setWindowTitle("qCreativeTools");
    mainWindow.showMaximized();

 */
    SettingsWindow settingsWindow;
    settingsWindow.setWindowTitle("qCreativeTools");
    settingsWindow.showMaximized();

    return QApplication::exec();
}