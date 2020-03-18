#pragma once

#include <QString>
#include <QVector2D>
#include <QVector4D>

class Shape
{
public:
    QString type;
    QVector2D position;
    QVector4D color;
};

class Rect : public Shape
{
public:
    float width;
    float height;
};

class Circle : public Shape
{
public:
    float radius;
};