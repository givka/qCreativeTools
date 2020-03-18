#include "draw_area.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QDebug>

DrawArea::DrawArea() :
        QWidget(),
        scene(new QGraphicsScene)
{
    // auto rect = scene->addRect(0, 0, 10, 10, QPen(Qt::yellow), QBrush(Qt::yellow));
    // auto circle = scene->addEllipse(-100, 0, 10, 10, QPen(Qt::red), QBrush(Qt::red));

    auto view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing, true);

    auto layout = new QHBoxLayout;
    layout->addWidget(view);
    setLayout(layout);
}

void DrawArea::drawShapes(const QVector4D &bgColor, const QVector<Shape *> &shapes)
{
    auto bgC = QColor(255 * bgColor.x(), 255 * bgColor.y(),
                      255 * bgColor.z(), 255 * bgColor.w());

    qDebug() << bgColor;
    qDebug() << bgC;

    scene->clear();
    scene->setBackgroundBrush(QBrush(bgC));

    for (const auto &shape: shapes) {

        auto color = QColor(255 * shape->color.x(), 255 * shape->color.y(),
                            255 * shape->color.z(), 255 * shape->color.w());
        if (shape->type == "Circle") {
            auto circle = static_cast<Circle *>(shape);
            scene->addEllipse(circle->position.x(), circle->position.x(),
                              circle->radius, circle->radius,
                              QPen(color), QBrush(color));
        }
        if (shape->type == "Rect") {
            auto rect = static_cast<Rect *>(shape);
            scene->addRect(rect->position.x(), rect->position.x(),
                           rect->width, rect->height,
                           QPen(color), QBrush(color));
        }
    }
}
