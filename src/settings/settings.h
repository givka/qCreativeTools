#pragma once

#include <QTreeView>
#include <QVector2D>
#include <QVector4D>
#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QLabel>
#include <QTreeWidget>

class Settings : public QWidget
{
Q_OBJECT

public :
    Settings();

    QGraphicsScene *getScene() { return scene; }

private:

    enum ShapeType
    {
        Rect = 3,
        Circle = 4
    };

    void loadJson();

    void saveJson();

    static QColor arrayToQColor(const QJsonArray &jsonArray);

    QGraphicsScene *scene;

    void addColorColumn(QHBoxLayout *hLayout, const QString &name, double value,
                        const std::function<void(double)> &function);

    void createShapeRow(QTreeWidgetItem *s, QGraphicsItem *graphicsItem);

    QTreeWidget *tree;
};

