#pragma once

#include <QTreeView>
#include <QVector2D>
#include <QVector4D>

class Settings : public QWidget
{
public :
    Settings();

private :

    struct Shape
    {
        QString type;
        QVector2D position;
        QVector4D color;
    };

    struct Rect : public Shape
    {
        float width;
        float height;
    };

    struct Circle : public Shape
    {
        float radius;
    };

    QVector<Shape *> shapes{};

    void load();

    static QVector4D arrayToQColor(const QJsonArray &jsonArray);

    QVector4D bgColor;
};


