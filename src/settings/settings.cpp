#include "settings.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QJsonArray>
#include <QImage>
#include <QStandardItemModel>
#include <QHBoxLayout>
#include <QGraphicsItem>
#include <QTreeWidget>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QPushButton>

Settings::Settings() : QWidget(),
                       tree(new QTreeWidget), scene(new QGraphicsScene)
{
    loadJson();

    /*
    auto shapeTypes = QMap<QString, ShapeType>();
    shapeTypes.insert("Rect", ShapeType::Rect);
    shapeTypes.insert("Circle", ShapeType::Circle);
    */

    tree->setHeaderHidden(true);
    tree->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
    tree->setFocusPolicy(Qt::FocusPolicy::NoFocus);

    auto bg = new QTreeWidgetItem(tree);
    bg->setExpanded(true);
    bg->setText(0, "background color");

    {
        auto item = new QTreeWidgetItem(bg);

        auto widget = new QWidget;
        auto hLayout = new QHBoxLayout;

        auto image = new QImage(QSize(1, 1), QImage::Format_RGBA8888);
        image->setPixelColor(QPoint(0, 0), scene->backgroundBrush().color());
        auto imageLabel = new QLabel;
        imageLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
        imageLabel->setPixmap(QPixmap::fromImage(*image).scaled(QSize(10, 10)));
        hLayout->addWidget(imageLabel);

        addColorColumn(hLayout, "r", scene->backgroundBrush().color().redF(), 0, 1,
                       [this, image, imageLabel](double value) {
                           auto c = scene->backgroundBrush().color();
                           c.setRedF(value);
                           scene->setBackgroundBrush(QBrush(c));
                           image->setPixelColor(QPoint(0, 0), scene->backgroundBrush().color());
                           imageLabel->setPixmap(QPixmap::fromImage(*image).scaled(QSize(10, 10)));
                       });

        addColorColumn(hLayout, "g", scene->backgroundBrush().color().greenF(), 0, 1,
                       [this, image, imageLabel](double value) {
                           auto c = scene->backgroundBrush().color();
                           c.setGreenF(value);
                           scene->setBackgroundBrush(QBrush(c));
                           image->setPixelColor(QPoint(0, 0), scene->backgroundBrush().color());
                           imageLabel->setPixmap(QPixmap::fromImage(*image).scaled(QSize(10, 10)));
                       });

        addColorColumn(hLayout, "b", scene->backgroundBrush().color().blueF(), 0, 1,
                       [this, image, imageLabel](double value) {
                           auto c = scene->backgroundBrush().color();
                           c.setBlueF(value);
                           scene->setBackgroundBrush(QBrush(c));
                           image->setPixelColor(QPoint(0, 0), scene->backgroundBrush().color());
                           imageLabel->setPixmap(QPixmap::fromImage(*image).scaled(QSize(10, 10)));
                       });

        addColorColumn(hLayout, "a", scene->backgroundBrush().color().alphaF(), 0, 1,
                       [this, image, imageLabel](double value) {
                           auto c = scene->backgroundBrush().color();
                           c.setAlphaF(value);
                           scene->setBackgroundBrush(QBrush(c));
                           image->setPixelColor(QPoint(0, 0), scene->backgroundBrush().color());
                           imageLabel->setPixmap(QPixmap::fromImage(*image).scaled(QSize(10, 10)));
                       });

        widget->setLayout(hLayout);
        tree->setItemWidget(item, 0, widget);
    }

    auto s = new QTreeWidgetItem(tree);
    s->setText(0, "shapes");
    s->setExpanded(true);

    for (auto graphicsItem : scene->items()) {
        createShapeRow(s, graphicsItem);
    }

    {
        auto newShape = new QTreeWidgetItem(tree);
        auto widget = new QWidget;
        auto hLayout = new QHBoxLayout;
        auto comboBox = new QComboBox;
        comboBox->addItems(QStringList{ "Rect", "Circle" });
        comboBox->setCurrentText("Rect");
        auto button = new QPushButton("Add a new");
        connect(button, &QAbstractButton::clicked, this, [this, comboBox, s]() {
            auto type = comboBox->currentText();
            if (type == "Rect") {
                auto graphicsItem = scene->addRect(QRectF(0, 0, 10, 10),
                                                   QPen(QColor(255, 255, 255)),
                                                   QBrush(QColor(255, 255, 255)));
                createShapeRow(s, graphicsItem);
            } else if (type == "Circle") {
                float width = 10;
                float height = 10;
                auto graphicsItem = scene->addEllipse(QRectF(0, 0, 10, 10),
                                                      QPen(QColor(255, 255, 255)),
                                                      QBrush(QColor(255, 255, 255)));
                createShapeRow(s, graphicsItem);
            }
        });
        hLayout->addWidget(button);
        hLayout->addWidget(comboBox);
        widget->setLayout(hLayout);
        tree->setItemWidget(newShape, 0, widget);
    }

    auto button = new QPushButton("Save");
    connect(button, &QPushButton::clicked, this, [this]() { saveJson(); });

    auto layout = new QVBoxLayout;
    layout->addWidget(tree);
    layout->addWidget(button);
    setLayout(layout);
}

QColor Settings::arrayToQColor(const QJsonArray &jsonArray)
{
    auto color = QColor();
    color.setRgbF(jsonArray[0].toDouble(),
                  jsonArray[1].toDouble(),
                  jsonArray[2].toDouble(),
                  jsonArray[3].toDouble());
    return color;
}

void Settings::addColorColumn(QHBoxLayout *hLayout, const QString &name, double value, double min,
                              double max, const std::function<void(double)> &function)
{

    auto label = new QLabel(name);
    label->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    auto spinBox = new QDoubleSpinBox();
    spinBox->setRange(min, max);
    spinBox->setDecimals(1);
    spinBox->setSingleStep(0.1);
    spinBox->setValue(value);
    connect(spinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            [this, spinBox, function]() {
                function(spinBox->value());
            });
    hLayout->addWidget(label);
    hLayout->addWidget(spinBox);
    hLayout->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
}

void Settings::createShapeRow(QTreeWidgetItem *s, QGraphicsItem *graphicsItem)
{
    static int id = 0;

    auto shape = qgraphicsitem_cast<QAbstractGraphicsShapeItem *>(graphicsItem);
    auto type = shape->type();
    if (type != ShapeType::Rect && type != ShapeType::Circle) {
        qWarning("incorrect type");
        return;
    }

    auto typeName = type == ShapeType::Rect ? QString("Rect") : QString("Circle");

    auto shapeParent = new QTreeWidgetItem(s);
    auto w = new QWidget;
    auto l = new QHBoxLayout;

    auto ql = new QLabel(QString("%1: %2").arg(QString::number(id++), typeName));
    auto b = new QPushButton("Delete");
    l->addWidget(ql);
    l->addWidget(b);
    w->setLayout(l);
    tree->setItemWidget(shapeParent, 0, w);
    connect(b, &QPushButton::clicked, this, [this, shapeParent, s, graphicsItem]() {
        s->removeChild(shapeParent);
        scene->removeItem(graphicsItem);
    });


    //type
    {
        auto treeItem = new QTreeWidgetItem(shapeParent);
        auto widget = new QWidget;
        auto label = new QLabel("type");
        auto typeLineEdit = new QLineEdit(typeName);
        typeLineEdit->setReadOnly(true);

        auto hLayout = new QHBoxLayout;
        hLayout->addWidget(label);
        hLayout->addWidget(typeLineEdit);
        widget->setLayout(hLayout);
        tree->setItemWidget(treeItem, 0, widget);
    }

    // color
    {
        auto treeItem = new QTreeWidgetItem(shapeParent);
        treeItem->setText(0, "color");
        treeItem = new QTreeWidgetItem(treeItem);

        auto widget = new QWidget;
        auto hLayout = new QHBoxLayout;

        auto image = new QImage(QSize(1, 1), QImage::Format_RGBA8888);
        image->setPixelColor(QPoint(0, 0), shape->brush().color());
        auto imageLabel = new QLabel;
        imageLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
        imageLabel->setPixmap(QPixmap::fromImage(*image).scaled(QSize(10, 10)));
        hLayout->addWidget(imageLabel);

        addColorColumn(hLayout, "r", shape->brush().color().redF(), 0, 1,
                       [shape, image, imageLabel](double value) {
                           auto c = shape->brush().color();
                           c.setRedF(value);
                           shape->setBrush(QBrush(c));
                           shape->setPen(QPen(c));
                           image->setPixelColor(0, 0, shape->brush().color());
                           imageLabel->setPixmap(QPixmap::fromImage(*image).scaled(QSize(10, 10)));
                       });

        addColorColumn(hLayout, "g", shape->brush().color().greenF(), 0, 1,
                       [shape, image, imageLabel](double value) {
                           auto c = shape->brush().color();
                           c.setGreenF(value);
                           shape->setBrush(QBrush(c));
                           shape->setPen(QPen(c));
                           image->setPixelColor(0, 0, shape->brush().color());
                           imageLabel->setPixmap(QPixmap::fromImage(*image).scaled(QSize(10, 10)));
                       });
        addColorColumn(hLayout, "b", shape->brush().color().blueF(), 0, 1,
                       [shape, image, imageLabel](double value) {
                           auto c = shape->brush().color();
                           c.setBlueF(value);
                           shape->setBrush(QBrush(c));
                           shape->setPen(QPen(c));
                           image->setPixelColor(0, 0, shape->brush().color());
                           imageLabel->setPixmap(QPixmap::fromImage(*image).scaled(QSize(10, 10)));
                       });
        addColorColumn(hLayout, "a", shape->brush().color().alphaF(), 0, 1,
                       [shape, image, imageLabel](double value) {
                           auto c = shape->brush().color();
                           c.setAlphaF(value);
                           shape->setBrush(QBrush(c));
                           shape->setPen(QPen(c));
                           image->setPixelColor(0, 0, shape->brush().color());
                           imageLabel->setPixmap(QPixmap::fromImage(*image).scaled(QSize(10, 10)));
                       });

        widget->setLayout(hLayout);
        tree->setItemWidget(treeItem, 0, widget);
    }

    if (auto rect = qgraphicsitem_cast<QGraphicsRectItem *>(shape)) {
        // width
        {
            auto treeItem = new QTreeWidgetItem(shapeParent);
            auto widget = new QWidget;
            auto hLayout = new QHBoxLayout;

            addColorColumn(hLayout, "width", rect->rect().width(), 0, 1000,
                           [rect](double value) {
                               auto r = rect->rect();
                               r.setWidth(value);
                               rect->setRect(r);
                           });

            widget->setLayout(hLayout);
            tree->setItemWidget(treeItem, 0, widget);
        }
        // height
        {
            auto treeItem = new QTreeWidgetItem(shapeParent);
            auto widget = new QWidget;
            auto hLayout = new QHBoxLayout;

            addColorColumn(hLayout, "height", rect->rect().height(), 0, 1000,
                           [rect](double value) {
                               auto r = rect->rect();
                               r.setHeight(value);
                               rect->setRect(r);
                           });

            widget->setLayout(hLayout);
            tree->setItemWidget(treeItem, 0, widget);
        }
        // position
        {
            auto treeItem = new QTreeWidgetItem(shapeParent);
            treeItem->setText(0, "position");

            treeItem = new QTreeWidgetItem(treeItem);

            auto widget = new QWidget;
            auto hLayout = new QHBoxLayout;

            addColorColumn(hLayout, "x", rect->rect().x(), 0, 1000,
                           [rect](double value) {
                               auto r = rect->rect();
                               r.moveLeft(value);
                               rect->setRect(r);
                           });

            addColorColumn(hLayout, "y", rect->rect().y(), 0, 1000,
                           [rect](double value) {
                               auto r = rect->rect();
                               r.moveTop(value);
                               rect->setRect(r);
                           });
            widget->setLayout(hLayout);
            tree->setItemWidget(treeItem, 0, widget);
        }
    }

    if (auto circle = qgraphicsitem_cast<QGraphicsEllipseItem *>(shape)) {
        // radius
        {
            auto treeItem = new QTreeWidgetItem(shapeParent);
            auto widget = new QWidget;
            auto hLayout = new QHBoxLayout;

            addColorColumn(hLayout, "radius", circle->rect().width() / 2, 0, 1000,
                           [circle](double value) {
                               auto r = circle->rect();
                               r.setWidth(value * 2);
                               r.setHeight(value * 2);
                               circle->setRect(r);
                           });

            widget->setLayout(hLayout);
            tree->setItemWidget(treeItem, 0, widget);
        }
        // position
        {
            auto treeItem = new QTreeWidgetItem(shapeParent);
            treeItem->setText(0, "position");

            treeItem = new QTreeWidgetItem(treeItem);

            auto widget = new QWidget;
            auto hLayout = new QHBoxLayout;

            addColorColumn(hLayout, "x", circle->rect().x(), 0, 1000,
                           [circle](double value) {
                               auto r = circle->rect();
                               r.moveLeft(value);
                               circle->setRect(r);
                           });

            addColorColumn(hLayout, "y", circle->rect().y(), 0, 1000,
                           [circle](double value) {
                               auto r = circle->rect();
                               r.moveTop(value);
                               circle->setRect(r);
                           });

            widget->setLayout(hLayout);
            tree->setItemWidget(treeItem, 0, widget);
        }
    }
}

void Settings::loadJson()
{
    QFile loadFile("/Users/artich/dev/qCreativeTools/resources/settings-save.json");

    if (!loadFile.open(QIODevice::ReadOnly))
        qWarning("ReadOnly Couldn't open settings file.");

    QJsonDocument loadDoc = QJsonDocument::fromJson(loadFile.readAll());

    auto json = loadDoc.object();

    scene->setBackgroundBrush(QBrush(arrayToQColor(json["bg_color"].toArray())));

    auto shapesArray = json["shapes"].toArray();

    for (const auto &shape : shapesArray) {
        auto shapeObject = shape.toObject();
        auto type = shapeObject["type"].toString();

        if (type == "Circle") {
            auto color = arrayToQColor(shapeObject["color"].toArray());
            auto radius = (float) shapeObject["radius"].toDouble();
            auto width = 2 * radius;
            auto x = (float) shapeObject["position"].toArray()[0].toDouble();
            auto y = (float) shapeObject["position"].toArray()[1].toDouble();

            scene->addEllipse(QRectF(x, y, width, width), QPen(color), QBrush(color));
        } else if (type == "Rect") {
            auto color = arrayToQColor(shapeObject["color"].toArray());
            auto width = (float) shapeObject["width"].toDouble();
            auto height = (float) shapeObject["height"].toDouble();
            auto x = (float) shapeObject["position"].toArray()[0].toDouble();
            auto y = (float) shapeObject["position"].toArray()[1].toDouble();

            scene->addRect(QRectF(x, y, width, height), QPen(color), QBrush(color));
        }
    }
}

void Settings::saveJson()
{
    QFile saveFile("/Users/artich/dev/qCreativeTools/resources/settings-save.json");

    if (!saveFile.open(QIODevice::WriteOnly))
        qWarning("WriteOnly Couldn't open settings file.");

    auto object = QJsonObject();

    object["bg_color"] = QJsonArray{ scene->backgroundBrush().color().redF(),
                                     scene->backgroundBrush().color().greenF(),
                                     scene->backgroundBrush().color().blueF(),
                                     scene->backgroundBrush().color().alphaF() };

    auto shapes = QJsonArray();

    for (const auto &item : scene->items()) {
        auto obj = QJsonObject();
        auto shape = qgraphicsitem_cast<QAbstractGraphicsShapeItem *>(item);
        obj["color"] = QJsonArray{ shape->brush().color().redF(),
                                   shape->brush().color().greenF(),
                                   shape->brush().color().blueF(),
                                   shape->brush().color().alphaF() };

        if (shape->type() == ShapeType::Rect) {
            auto rect = qgraphicsitem_cast<QGraphicsRectItem *>(item);
            obj["type"] = "Rect";
            obj["width"] = rect->rect().width();
            obj["height"] = rect->rect().height();
            obj["position"] = QJsonArray{ rect->rect().x(), rect->rect().y() };
        } else if (shape->type() == ShapeType::Circle) {
            auto circle = qgraphicsitem_cast<QGraphicsEllipseItem *>(item);
            obj["type"] = "Circle";
            obj["radius"] = circle->rect().width() / 2;
            obj["position"] = QJsonArray{ circle->rect().x(), circle->rect().y() };
        }

        shapes.append(obj);
    }

    object["shapes"] = shapes;

    saveFile.write(QJsonDocument(object).toJson());
}