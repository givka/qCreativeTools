#pragma once

#include <QTreeView>
#include <QVector2D>
#include <QVector4D>
#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QLabel>

class Settings : public QWidget
{
Q_OBJECT

public :
    Settings();

    QGraphicsScene *getScene() { return scene; }

private:

    enum ItemType
    {
        Rect = 3,
        Circle = 4
    };

    void load();

    static QColor arrayToQColor(const QJsonArray &jsonArray);

    QGraphicsScene *scene;

    void addColorColumn(QHBoxLayout *hLayout, const QString &name, double value, QLabel *imageLabel,
                        QImage *image, const std::function<void(double)> &function);
};

