#pragma once

#include <QTreeView>
#include <QVector2D>
#include <QVector4D>
#include <QGraphicsScene>

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
};


