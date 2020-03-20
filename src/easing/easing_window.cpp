#include <QDebug>
#include <QPushButton>

#include "easing_window.h"
#include "../utility.h"

EasingWindow::EasingWindow()
        : QMainWindow(),
          list(new QListWidget), settings(new QWidget), previewImage(new QLabel),
          typeLineEdit(new QLineEdit),
          amplitude(new QDoubleSpinBox), overshoot(new QDoubleSpinBox), period(new QDoubleSpinBox),
          amplitudeWidget(new QWidget), overshootWidget(new QWidget), periodWidget(new QWidget)
{
    auto scene = new AnimatedLabel;
    animation = new QPropertyAnimation(scene, "color");

    initList();
    settings = new QWidget;
    initSettings();

    list->setCurrentRow(0);
    drawPreview();
    updateSettings();

    scene->setAlignment(Qt::AlignCenter);
    auto image = QImage(1, 1, QImage::Format_RGBA8888);
    image.fill(QColor(255, 0, 0, 255));
    scene->setPixmap(QPixmap::fromImage(image).scaled(200, 200));

    auto slider = new QSlider(Qt::Horizontal);
    auto play = new QPushButton(QIcon(style()->standardIcon(QStyle::SP_MediaPlay)), "");
    connect(play, &QPushButton::clicked, this, [this, play]() {
        isPlayed = !isPlayed;
        animation->setPaused(!isPlayed);
        play->setIcon(QIcon(style()->standardIcon(
                isPlayed ? QStyle::SP_MediaPlay : QStyle::SP_MediaPause)));
    });

    animation->setStartValue(QColor(0, 0, 0, 255));
    animation->setEndValue(QColor(255, 255, 255, 255));
    animation->setDuration(2000);
    animation->setLoopCount(-1); // forever
    animation->start();

    auto timer = new QTimer(this);
    timer->setInterval(1);
    timer->start();

    connect(timer, &QTimer::timeout, this, [this, slider]() {
        if (animation->state() == QAbstractAnimation::State::Paused) return;
        auto ratio = (float) animation->currentLoopTime() / (float) animation->duration();
        slider->setValue((int) (100 * ratio));
        drawPreview();
    });

    connect(slider, &QSlider::valueChanged, this, [this](int value) {
        if (animation->state() == QAbstractAnimation::State::Running) return;
        auto ratio = (float) value / 100;
        animation->setCurrentTime((int) (ratio * (float) animation->duration()));
        drawPreview();
    });

    previewImage->setAlignment(Qt::AlignCenter);

    connect(list, &QListWidget::currentRowChanged, this, [this, slider](int row) {
        currentCurve = createCurve(static_cast<QEasingCurve::Type>(row));
        animation->setEasingCurve(currentCurve);
        animation->setCurrentTime(0);
        slider->setValue(0);
        drawPreview();
        updateSettings();
    });
    connect(amplitude, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            [this](double value) {
                currentCurve.setAmplitude(value);
                animation->setEasingCurve(currentCurve);
                drawPreview();
            });
    connect(overshoot, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            [this](double value) {
                currentCurve.setOvershoot(value);
                animation->setEasingCurve(currentCurve);
                drawPreview();
            });
    connect(period, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            [this](double value) {
                currentCurve.setPeriod(value);
                animation->setEasingCurve(currentCurve);
                drawPreview();
            });

    auto sceneGb = Utility::createGroupBox("Scene", new QVBoxLayout,
                                           QList<QWidget *>{ scene, slider, play });
    auto previewGb = Utility::createGroupBox("Preview", new QVBoxLayout,
                                             QList<QWidget *>{ previewImage });
    auto settingsGb = Utility::createGroupBox("Settings", new QHBoxLayout,
                                              QList<QWidget *>{ settings });
    auto listGb = Utility::createGroupBox("Easing Curves", new QHBoxLayout,
                                          QList<QWidget *>{ list });

    auto layout = new QGridLayout;
    layout->addWidget(sceneGb, 0, 0, 2, 2);
    layout->addWidget(listGb, 2, 0, 1, 2);
    layout->addWidget(previewGb, 0, 2, 2, 1);
    layout->addWidget(settingsGb, 2, 2, 1, 1);

    // to keep the rows and columns the same size.
//    for (int i = 0; i < layout->rowCount(); i++)
//       layout->setRowStretch(i, 1);
    for (int i = 0; i < layout->columnCount(); i++)
        layout->setColumnStretch(i, 1);

    auto window = new QWidget;
    window->setLayout(layout);
    setCentralWidget(window);
}

QPixmap EasingWindow::createEasingPixmap(const QEasingCurve &curve, int size)
{
    auto image = QImage(QSize(size, size), QImage::Format_RGBA8888);
    image.fill(QColor(0, 0, 0, 128));

    QPainter painter(&image);
    painter.setPen(QPen(QColor(255, 255, 255, 255)));
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);

    for (int x = 0; x < size; ++x) {
        int y1 = (int) ((float) size * curve.valueForProgress((float) x / (float) size));
        int y2 = (int) ((float) size * curve.valueForProgress((float) (x + 1) / (float) size));

        painter.drawLine(x / 2 + size / 4, y1 / 2 + size / 4,
                         (x + 1) / 2 + size / 4, y2 / 2 + size / 4);
    }

    painter.setBrush(QBrush(QColor(255, 128, 128, 255)));

    if (animation->state() != QAbstractAnimation::State::Stopped) {
        auto ratio = (float) animation->currentLoopTime() / animation->duration();
        int y1 = (int) ((float) size * curve.valueForProgress(ratio));
        painter.drawEllipse(ratio * size / 2 + size / 4 - 5, y1 / 2 + size / 4 - 5, 10, 10);
    }

    painter.setPen(QPen(painter.brush(), 1, Qt::DotLine));
    painter.drawLine(size / 4, 0, size / 4, size);
    painter.drawLine(0, 3 * size / 4, size, 3 * size / 4);

    painter.drawLine(size / 4, size / 4, 3 * size / 4, size / 4);
    painter.drawLine(3 * size / 4, size / 4, 3 * size / 4, 3 * size / 4);

    return QPixmap::fromImage(image);
}

void EasingWindow::initList()
{
    list->setIconSize(QSize(EasingWindow::Size, EasingWindow::Size));
    list->setViewMode(QListView::ViewMode::IconMode);
    list->setResizeMode(QListView::ResizeMode::Adjust);
    list->setSpacing(11);
    list->setMovement(QListView::Movement::Static);
    list->setWordWrap(true);

    // - 1 because we remove QEasingCurve::Type::Custom which is not a type
    int nCurveTypes = (int) QEasingCurve::Type::NCurveTypes - 1;

    for (int i = 0; i < nCurveTypes; ++i) {
        auto type = static_cast<QEasingCurve::Type>(i);
        auto name = QString(QVariant::fromValue(type).toString());
        auto item = new QListWidgetItem;
        item->setText(name);
        item->setIcon(EasingWindow::createEasingPixmap(createCurve(type), EasingWindow::Size));
        list->addItem(item);
    }
}

void EasingWindow::initSettings()
{
    auto vLayout = new QVBoxLayout;
    {
        auto widget = new QWidget;
        auto layout = new QHBoxLayout;
        layout->addWidget(new QLabel("type"));
        layout->addWidget(typeLineEdit);
        layout->setStretch(0, 1);
        layout->setStretch(1, 1);
        widget->setLayout(layout);
        vLayout->addWidget(widget);
    }
    {
        auto layout = new QHBoxLayout;
        layout->addWidget(new QLabel("amplitude"));
        layout->addWidget(amplitude);
        layout->setStretch(0, 1);
        layout->setStretch(1, 1);
        amplitude->setSingleStep(0.1);
        amplitudeWidget->setLayout(layout);
        vLayout->addWidget(amplitudeWidget);
    }
    {
        auto layout = new QHBoxLayout;
        layout->addWidget(new QLabel("overshoot"));
        layout->addWidget(overshoot);
        layout->setStretch(0, 1);
        layout->setStretch(1, 1);
        overshoot->setSingleStep(0.1);
        overshootWidget->setLayout(layout);
        vLayout->addWidget(overshootWidget);
    }
    {
        auto layout = new QHBoxLayout;
        layout->addWidget(new QLabel("period"));
        layout->addWidget(period);
        layout->setStretch(0, 1);
        layout->setStretch(1, 1);
        period->setSingleStep(0.1);
        periodWidget->setLayout(layout);
        vLayout->addWidget(periodWidget);
    }
    vLayout->addStretch();
    settings->setLayout(vLayout);
}

void EasingWindow::drawPreview()
{
    auto pixmap = EasingWindow::createEasingPixmap(currentCurve, 400);
    previewImage->setPixmap(pixmap);
}

void EasingWindow::updateSettings()
{
    amplitudeWidget->setEnabled(false);
    overshootWidget->setEnabled(false);
    periodWidget->setEnabled(false);

    static auto amplitudeTypes = QList<QEasingCurve::Type>{
            QEasingCurve::InBounce, QEasingCurve::OutBounce, QEasingCurve::InOutBounce,
            QEasingCurve::OutInBounce, QEasingCurve::InElastic, QEasingCurve::OutElastic,
            QEasingCurve::InOutElastic, QEasingCurve::OutInElastic
    };
    static auto overshootTypes = QList<QEasingCurve::Type>{
            QEasingCurve::InBack, QEasingCurve::OutBack,
            QEasingCurve::InOutBack, QEasingCurve::OutInBack
    };
    static auto periodTypes = QList<QEasingCurve::Type>{
            QEasingCurve::InElastic, QEasingCurve::OutElastic,
            QEasingCurve::InOutElastic, QEasingCurve::OutInElastic
    };

    if (amplitudeTypes.contains(currentCurve.type()))
        amplitudeWidget->setEnabled(true);
    if (overshootTypes.contains(currentCurve.type()))
        overshootWidget->setEnabled(true);
    if (periodTypes.contains(currentCurve.type()))
        periodWidget->setEnabled(true);

    typeLineEdit->setText(QString(QVariant::fromValue(currentCurve.type()).toString()));
    amplitude->setValue(currentCurve.amplitude());
    overshoot->setValue(currentCurve.overshoot());
    period->setValue(currentCurve.period());
}

QEasingCurve EasingWindow::createCurve(QEasingCurve::Type type)
{
    auto curve = QEasingCurve(type);
    if (curve.type() == QEasingCurve::BezierSpline) {
        curve.addCubicBezierSegment(QPointF(0.4, 0.1), QPointF(0.6, 0.9), QPointF(1.0, 1.0));
    } else if (curve.type() == QEasingCurve::TCBSpline) {
        curve.addTCBSegment(QPointF(0.0, 0.0), 0, 0, 0);
        curve.addTCBSegment(QPointF(0.3, 0.4), 0.2, 1, -0.2);
        curve.addTCBSegment(QPointF(0.7, 0.6), -0.2, 1, 0.2);
        curve.addTCBSegment(QPointF(1.0, 1.0), 0, 0, 0);
    }
    return curve;
}
