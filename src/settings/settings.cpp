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

Settings::Settings() : QWidget(), scene(new QGraphicsScene)
{
    load();

    auto tree = new QTreeWidget();
    tree->setHeaderHidden(true);
    tree->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
    tree->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    tree->setStyleSheet("QTreeWidget::item{padding:5px;}");

    auto bg = new QTreeWidgetItem(tree);
    bg->setExpanded(true);
    bg->setText(0, "background color");

    {
        auto item = new QTreeWidgetItem(bg);

        auto widget = new QWidget;
        auto hLayout = new QHBoxLayout;

        auto image = QImage(QSize(1, 1), QImage::Format_RGBA8888);
        image.setPixelColor(QPoint(0, 0), scene->backgroundBrush().color());
        auto label = new QLabel;
        label->setPixmap(QPixmap::fromImage(image).scaled(QSize(10, 10)));
        hLayout->addWidget(label);

        label = new QLabel("r");
        auto lineEdit = new QLineEdit();
        lineEdit->setText(QString::number(scene->backgroundBrush().color().red()));
        connect(lineEdit, &QLineEdit::editingFinished, this, [this, lineEdit]() {
            auto c = scene->backgroundBrush().color();
            c.setRed(lineEdit->text().toInt());
            scene->setBackgroundBrush(QBrush(c));
        });
        hLayout->addWidget(label);
        hLayout->addWidget(lineEdit);

        label = new QLabel("g");
        lineEdit = new QLineEdit();
        lineEdit->setText(QString::number(scene->backgroundBrush().color().green()));
        connect(lineEdit, &QLineEdit::editingFinished, this, [this, lineEdit]() {
            auto c = scene->backgroundBrush().color();
            c.setGreen(lineEdit->text().toInt());
            scene->setBackgroundBrush(QBrush(c));
        });
        hLayout->addWidget(label);
        hLayout->addWidget(lineEdit);

        label = new QLabel("b");
        lineEdit = new QLineEdit();
        lineEdit->setText(QString::number(scene->backgroundBrush().color().blue()));
        connect(lineEdit, &QLineEdit::editingFinished, this, [this, lineEdit]() {
            auto c = scene->backgroundBrush().color();
            c.setBlue(lineEdit->text().toInt());
            scene->setBackgroundBrush(QBrush(c));
        });
        hLayout->addWidget(label);
        hLayout->addWidget(lineEdit);

        label = new QLabel("a");
        lineEdit = new QLineEdit();
        lineEdit->setText(QString::number(scene->backgroundBrush().color().alpha()));
        connect(lineEdit, &QLineEdit::editingFinished, this, [this, lineEdit]() {
            auto c = scene->backgroundBrush().color();
            c.setAlpha(lineEdit->text().toInt());
            scene->setBackgroundBrush(QBrush(c));
        });
        hLayout->addWidget(label);
        hLayout->addWidget(lineEdit);
        hLayout->setMargin(0);
        widget->setLayout(hLayout);
        tree->setItemWidget(item, 0, widget);
    }

    auto s = new QTreeWidgetItem(tree);
    s->setText(0, "shapes");
    s->setExpanded(true);

    int i = 0;
    for (auto &graphicsItem : scene->items()) {
        auto shape = qgraphicsitem_cast<QAbstractGraphicsShapeItem *>(graphicsItem);
        auto shapeParent = new QTreeWidgetItem(s);
        shapeParent->setText(0, "shape " + QString::number(++i));
        shapeParent->setExpanded(true);

        qDebug() << shape->type();
        auto type = shape->type();
        if (type != ItemType::Rect && type != ItemType::Circle) {
            qWarning("incorrect type");
            return;
        }

        auto typeName = type == ItemType::Rect ? QString("Rect") : QString("Circle");

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
            treeItem->setExpanded(true);

            treeItem = new QTreeWidgetItem(treeItem);

            auto widget = new QWidget;
            auto hLayout = new QHBoxLayout;

            auto image = new QImage(QSize(1, 1), QImage::Format_RGBA8888);
            image->setPixelColor(QPoint(0, 0), shape->brush().color());
            auto imageLabel = new QLabel;
            imageLabel->setPixmap(QPixmap::fromImage(*image).scaled(QSize(10, 10)));
            hLayout->addWidget(imageLabel);

            auto label = new QLabel("r");
            auto lineEdit = new QLineEdit();
            lineEdit->setText(QString::number(shape->brush().color().red()));
            connect(lineEdit, &QLineEdit::editingFinished, this,
                    [this, lineEdit, shape, image, imageLabel]() {
                        auto c = shape->brush().color();
                        c.setRed(lineEdit->text().toInt());
                        shape->setBrush(QBrush(c));
                        shape->setPen(QPen(c));
                        image->setPixelColor(QPoint(0, 0), shape->brush().color());
                        imageLabel->setPixmap(QPixmap::fromImage(*image).scaled(QSize(10, 10)));
                    });
            hLayout->addWidget(label);
            hLayout->addWidget(lineEdit);

            label = new QLabel("g");
            lineEdit = new QLineEdit();
            lineEdit->setText(QString::number(shape->brush().color().green()));
            connect(lineEdit, &QLineEdit::editingFinished, this,
                    [this, lineEdit, shape, image, imageLabel]() {
                        auto c = shape->brush().color();
                        c.setGreen(lineEdit->text().toInt());
                        shape->setBrush(QBrush(c));
                        shape->setPen(QPen(c));
                        image->setPixelColor(QPoint(0, 0), shape->brush().color());
                        imageLabel->setPixmap(QPixmap::fromImage(*image).scaled(QSize(10, 10)));
                    });
            hLayout->addWidget(label);
            hLayout->addWidget(lineEdit);

            label = new QLabel("b");
            lineEdit = new QLineEdit();
            lineEdit->setText(QString::number(shape->brush().color().blue()));
            connect(lineEdit, &QLineEdit::editingFinished, this,
                    [this, lineEdit, shape, image, imageLabel]() {
                        auto c = shape->brush().color();
                        c.setBlue(lineEdit->text().toInt());
                        shape->setBrush(QBrush(c));
                        shape->setPen(QPen(c));
                        image->setPixelColor(QPoint(0, 0), shape->brush().color());
                        imageLabel->setPixmap(QPixmap::fromImage(*image).scaled(QSize(10, 10)));
                    });
            hLayout->addWidget(label);
            hLayout->addWidget(lineEdit);

            label = new QLabel("a");
            lineEdit = new QLineEdit();
            lineEdit->setText(QString::number(shape->brush().color().alpha()));
            connect(lineEdit, &QLineEdit::editingFinished, this,
                    [this, lineEdit, shape, image, imageLabel]() {
                        auto c = shape->brush().color();
                        c.setAlpha(lineEdit->text().toInt());
                        shape->setBrush(QBrush(c));
                        shape->setPen(QPen(c));
                        image->setPixelColor(QPoint(0, 0), shape->brush().color());
                        imageLabel->setPixmap(QPixmap::fromImage(*image).scaled(QSize(10, 10)));
                    });
            hLayout->addWidget(label);
            hLayout->addWidget(lineEdit);

            hLayout->setMargin(0);
            widget->setLayout(hLayout);
            tree->setItemWidget(treeItem, 0, widget);
        }

        // circle radius
        if (shape->type() == ItemType::Circle) {
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
        if (shape->type() == ItemType::Rect) {
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
        if (shape->type() == ItemType::Rect) {
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
            treeItem->setExpanded(true);

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
        };
    }
}


