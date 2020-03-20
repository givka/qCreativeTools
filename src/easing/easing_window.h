#pragma once

#include <QMainWindow>
#include <QListWidget>
#include <QPainter>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QEasingCurve>
#include <QDebug>
#include <QLabel>
#include <QDoubleSpinBox>

class EasingWindow : public QMainWindow
{
public:
    EasingWindow();

private:

    static QPixmap createEasingPixmap(const QEasingCurve &curve, int size);

    void initList();

    void initSettings();

    void drawPreview();

    static constexpr int Size = 100;
    QWidget *settings;
    QListWidget *list;
    QLabel *previewImage;
    QEasingCurve currentCurve;
    QLineEdit *typeLineEdit;
    QDoubleSpinBox *amplitude;
    QDoubleSpinBox *overshoot;
    QDoubleSpinBox *period;
    QWidget *amplitudeWidget;
    QWidget *overshootWidget;
    QWidget *periodWidget;

    void updateSettings();
};


