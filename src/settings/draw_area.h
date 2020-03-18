#pragma once

#include <QWidget>
#include <QAbstractGraphicsShapeItem>
#include "settings.h"

class DrawArea : public QWidget
{
public:
    DrawArea();

public slots:

    void drawShapes(const QVector4D &bgColor, const QVector<Shape *> &shapes);

private:
    QGraphicsScene *scene;
};

