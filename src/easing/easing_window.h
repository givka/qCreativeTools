#pragma once

#include <QMainWindow>
#include <QListWidget>
#include <QPainter>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QTimer>
#include <QEasingCurve>
#include <QDebug>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QPropertyAnimation>

class EasingWindow : public QMainWindow
{
public:
    EasingWindow();

private:

     QPixmap createEasingPixmap(const QEasingCurve &curve, int size);

    void initList();

    void initSettings();

    void drawPreview();

    static constexpr int Size = 64;
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
    QPropertyAnimation *animation;
    bool isPlayed = true;

    void updateSettings();

    static QEasingCurve createCurve(QEasingCurve::Type type);
};

class AnimatedLabel : public QLabel
{

Q_OBJECT;
    Q_PROPERTY(QColor color
                       READ
                               color
                       WRITE
                       setColor);

public:
    explicit AnimatedLabel(QWidget *parent = nullptr)
    {
    }

    void setColor(const QColor &color)
    {
        setStyleSheet(QString("background-color: rgba(%1, %2, %3, %4);")
                              .arg(color.red())
                              .arg(color.green())
                              .arg(color.blue())
                              .arg(color.alpha()));
        auto image = QImage(1, 1, QImage::Format_RGBA8888);
        image.fill(QColor(255 - color.red(), 255 - color.green(), 255 - color.blue(), 255));
        setPixmap(QPixmap::fromImage(image).scaled(200, 200));
    }

    static QColor color()
    {
        return Qt::black; // getter is not really needed for now
    }
};
