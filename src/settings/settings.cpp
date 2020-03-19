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
    load();

    /*
    auto shapeTypes = QMap<QString, ShapeType>();
    shapeTypes.insert("Rect", ShapeType::Rect);
    shapeTypes.insert("Circle", ShapeType::Circle);
    */

    tree->setHeaderHidden(true);
    tree->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
    tree->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    tree->setStyleSheet("QTreeWidget{padding:5px;}");

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

        addColorColumn(hLayout, "r", scene->backgroundBrush().color().redF(),
                       [this, image, imageLabel](double value) {
                           auto c = scene->backgroundBrush().color();
                           c.setRedF(value);
                           scene->setBackgroundBrush(QBrush(c));
                           image->setPixelColor(QPoint(0, 0), scene->backgroundBrush().color());
                           imageLabel->setPixmap(QPixmap::fromImage(*image).scaled(QSize(10, 10)));
                       });

        addColorColumn(hLayout, "g", scene->backgroundBrush().color().greenF(),
                       [this, image, imageLabel](double value) {
                           auto c = scene->backgroundBrush().color();
                           c.setGreenF(value);
                           scene->setBackgroundBrush(QBrush(c));
                           image->setPixelColor(QPoint(0, 0), scene->backgroundBrush().color());
                           imageLabel->setPixmap(QPixmap::fromImage(*image).scaled(QSize(10, 10)));
                       });

        addColorColumn(hLayout, "b", scene->backgroundBrush().color().blueF(),
                       [this, image, imageLabel](double value) {
                           auto c = scene->backgroundBrush().color();
                           c.setBlueF(value);
                           scene->setBackgroundBrush(QBrush(c));
                           image->setPixelColor(QPoint(0, 0), scene->backgroundBrush().color());
                           imageLabel->setPixmap(QPixmap::fromImage(*image).scaled(QSize(10, 10)));
                       });

        addColorColumn(hLayout, "a", scene->backgroundBrush().color().alphaF(),
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
                auto graphicsItem = scene->addRect(QRectF(), QPen(QColor()), QBrush(QColor()));
                createShapeRow(s, graphicsItem);
            } else if (type == "Circle") {
                auto graphicsItem = scene->addEllipse(QRectF(), QPen(QColor()), QBrush(QColor()));
                createShapeRow(s, graphicsItem);
            }
        });
        hLayout->addWidget(button);
        hLayout->addWidget(comboBox);
        widget->setLayout(hLayout);
        tree->setItemWidget(newShape, 0, widget);
    }

    auto layout = new QHBoxLayout;
    layout->addWidget(tree);
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

void Settings::load()
{
    QFile loadFile("/Users/artich/dev/qCreativeTools/resources/settings.json");

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
            scene->addEllipse(x - width / 2, y - width / 2, width, width, QPen(color),
                              QBrush(color));
        } else if (type == "Rect") {
            auto color = arrayToQColor(shapeObject["color"].toArray());
            auto width = (float) shapeObject["width"].toDouble();
            auto height = (float) shapeObject["height"].toDouble();
            auto x = (float) shapeObject["position"].toArray()[0].toDouble();
            auto y = (float) shapeObject["position"].toArray()[1].toDouble();
            scene->addRect(x - width / 2, y - height / 2, width, height, QPen(color),
                           QBrush(color));
        }
    }
}

void Settings::addColorColumn(QHBoxLayout *hLayout, const QString &name, double value,
                              const std::function<void(double)> &function)
{

    auto label = new QLabel(name);
    label->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    auto spinBox = new QDoubleSpinBox();
    spinBox->setRange(0, 1);
    spinBox->setDecimals(1);
    spinBox->setSingleStep(0.1);
    spinBox->setValue(value);
    connect(spinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            [this, spinBox, function]() {
                function(spinBox->value());
            });
    hLayout->addWidget(label);
    hLayout->addWidget(spinBox);
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
    shapeParent->setText(0, QString("%1: %2").arg(QString::number(id++), typeName));

    //type
    {
        auto treeItem = new QTreeWidgetItem(shapeParent);
        auto widget = new QWidget;
        auto label = new QLabel("type");
        auto typeLineEdit = new QLineEdit(typeName);
        typeLineEdit->setReadOnly(true);

        auto hLayout = new QHBoxLayout;
        hLayout->setMargin(0);
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

        addColorColumn(hLayout, "r", shape->brush().color().redF(),
                       [shape, image, imageLabel](double value) {
                           auto c = shape->brush().color();
                           c.setRedF(value);
                           shape->setBrush(QBrush(c));
                           shape->setPen(QPen(c));
                           image->setPixelColor(0, 0, shape->brush().color());
                           imageLabel->setPixmap(QPixmap::fromImage(*image).scaled(QSize(10, 10)));
                       });

        addColorColumn(hLayout, "g", shape->brush().color().greenF(),
                       [shape, image, imageLabel](double value) {
                           auto c = shape->brush().color();
                           c.setGreenF(value);
                           shape->setBrush(QBrush(c));
                           shape->setPen(QPen(c));
                           image->setPixelColor(0, 0, shape->brush().color());
                           imageLabel->setPixmap(QPixmap::fromImage(*image).scaled(QSize(10, 10)));
                       });
        addColorColumn(hLayout, "b", shape->brush().color().blueF(),
                       [shape, image, imageLabel](double value) {
                           auto c = shape->brush().color();
                           c.setBlueF(value);
                           shape->setBrush(QBrush(c));
                           shape->setPen(QPen(c));
                           image->setPixelColor(0, 0, shape->brush().color());
                           imageLabel->setPixmap(QPixmap::fromImage(*image).scaled(QSize(10, 10)));
                       });
        addColorColumn(hLayout, "a", shape->brush().color().alphaF(),
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

    // circle radius
    if (shape->type() == ShapeType::Circle) {
        auto circle = qgraphicsitem_cast<QGraphicsEllipseItem *>(shape);
        auto treeItem = new QTreeWidgetItem(shapeParent);
        auto widget = new QWidget;
        auto label = new QLabel("radius");
        auto lineEdit = new QLineEdit;
        lineEdit->setText(QString::number(circle->rect().width()));
        connect(lineEdit, &QLineEdit::editingFinished, this, [this, lineEdit, circle]() {
            auto r = circle->rect();
            r.setWidth(lineEdit->text().toDouble() * 2);
            r.setHeight(lineEdit->text().toDouble() * 2);
            circle->setRect(r);
        });
        auto hLayout = new QHBoxLayout;
        hLayout->setMargin(0);
        hLayout->addWidget(label);
        hLayout->addWidget(lineEdit);
        widget->setLayout(hLayout);
        tree->setItemWidget(treeItem, 0, widget);
    }

    //rect width
    if (shape->type() == ShapeType::Rect) {
        auto rect = qgraphicsitem_cast<QGraphicsRectItem *>(shape);
        auto treeItem = new QTreeWidgetItem(shapeParent);
        auto widget = new QWidget;
        auto label = new QLabel("width");
        auto lineEdit = new QLineEdit;
        lineEdit->setText(QString::number(rect->rect().width()));
        connect(lineEdit, &QLineEdit::editingFinished, this, [this, lineEdit, rect]() {
            auto r = rect->rect();
            r.setWidth(lineEdit->text().toDouble());
            rect->setRect(r);
        });

        auto hLayout = new QHBoxLayout;
        hLayout->setMargin(0);
        hLayout->addWidget(label);
        hLayout->addWidget(lineEdit);
        widget->setLayout(hLayout);
        tree->setItemWidget(treeItem, 0, widget);
    }

    // rect height
    if (shape->type() == ShapeType::Rect) {
        auto rect = qgraphicsitem_cast<QGraphicsRectItem *>(shape);
        auto treeItem = new QTreeWidgetItem(shapeParent);
        auto widget = new QWidget;
        auto label = new QLabel("height");
        auto lineEdit = new QLineEdit;
        lineEdit->setText(QString::number(rect->rect().height()));
        connect(lineEdit, &QLineEdit::editingFinished, this, [this, lineEdit, rect]() {
            auto r = rect->rect();
            r.setHeight(lineEdit->text().toDouble());
            rect->setRect(r);
        });
        auto hLayout = new QHBoxLayout;
        hLayout->setMargin(0);
        hLayout->addWidget(label);
        hLayout->addWidget(lineEdit);
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

        auto label = new QLabel("x");
        auto lineEdit = new QLineEdit();
        lineEdit->setText(QString::number(shape->boundingRect().center().x()));

        hLayout->addWidget(label);
        hLayout->addWidget(lineEdit);

        label = new QLabel("y");
        lineEdit = new QLineEdit();
        lineEdit->setText(QString::number(shape->boundingRect().center().y()));

        hLayout->addWidget(label);
        hLayout->addWidget(lineEdit);

        hLayout->setMargin(0);
        widget->setLayout(hLayout);
        tree->setItemWidget(treeItem, 0, widget);
    }
}


