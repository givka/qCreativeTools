#include "settings.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QJsonArray>
#include <QImage>
#include <QStandardItemModel>
#include <QHBoxLayout>
#include <QTreeWidget>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>

Settings::Settings() : QWidget()
{
    load();

    auto tree = new QTreeWidget();
    tree->setHeaderHidden(true);
    tree->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
    tree->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    tree->setStyleSheet("QTreeWidget::item{padding:5px;}");
    auto bg = new QTreeWidgetItem(tree);
    bg->setText(0, "background color");
    {
        auto color = bgColor;


        auto item = new QTreeWidgetItem(bg);

        auto widget = new QWidget;
        auto hLayout = new QHBoxLayout;

        auto image = QImage(QSize(1, 1), QImage::Format_RGBA8888);
        image.setPixelColor(QPoint(0, 0),
                            QColor(255 * color.x(), 255 * color.y(), 255 * color.z(),
                                   255 * color.w()));
        auto label = new QLabel;
        label->setPixmap(
                QPixmap::fromImage(image).scaled(QSize(10, 10)));
        hLayout->addWidget(label);

        label = new QLabel("r");
        auto lineEdit = new QLineEdit();
        lineEdit->setText(QString::number(color.x()));
        lineEdit->setValidator(new QDoubleValidator(0, 1, 3, widget));
        hLayout->addWidget(label);
        hLayout->addWidget(lineEdit);

        label = new QLabel("g");
        lineEdit = new QLineEdit();
        lineEdit->setText(QString::number(color.y()));
        lineEdit->setValidator(new QDoubleValidator(0, 1, 3, widget));
        hLayout->addWidget(label);
        hLayout->addWidget(lineEdit);

        label = new QLabel("b");
        lineEdit = new QLineEdit();
        lineEdit->setText(QString::number(color.z()));
        lineEdit->setValidator(new QDoubleValidator(0, 1, 3, widget));
        hLayout->addWidget(label);
        hLayout->addWidget(lineEdit);

        label = new QLabel("a");
        lineEdit = new QLineEdit();
        lineEdit->setText(QString::number(color.w()));
        lineEdit->setValidator(new QDoubleValidator(0, 1, 3, widget));
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
    for (const auto &shape : shapes) {
        auto shapeParent = new QTreeWidgetItem(s);
        shapeParent->setText(0, "shape " + QString::number(++i));
        shapeParent->setExpanded(true);

        //type
        {
            auto item = new QTreeWidgetItem(shapeParent);
            auto widget = new QWidget;
            auto label = new QLabel("type");
            auto comboBox = new QComboBox;
            comboBox->addItems(QStringList{ "Rect", "Circle" });
            comboBox->setCurrentText(shape->type);
            auto hLayout = new QHBoxLayout;
            hLayout->setMargin(0);
            hLayout->addWidget(label);
            hLayout->addWidget(comboBox);
            widget->setLayout(hLayout);
            tree->setItemWidget(item, 0, widget);
        }

        // color
        {
            auto color = shape->color;
            auto item = new QTreeWidgetItem(shapeParent);
            item->setText(0, "color");
            item->setExpanded(true);

            item = new QTreeWidgetItem(item);

            auto widget = new QWidget;
            auto hLayout = new QHBoxLayout;

            auto image = QImage(QSize(1, 1), QImage::Format_RGBA8888);
            image.setPixelColor(QPoint(0, 0),
                                QColor(255 * color.x(), 255 * color.y(), 255 * color.z(),
                                       255 * color.w()));
            auto label = new QLabel;
            label->setPixmap(
                    QPixmap::fromImage(image).scaled(QSize(10, 10)));
            hLayout->addWidget(label);

            label = new QLabel("r");
            auto lineEdit = new QLineEdit();
            lineEdit->setText(QString::number(color.x()));
            lineEdit->setValidator(new QDoubleValidator(0, 1, 3, widget));
            hLayout->addWidget(label);
            hLayout->addWidget(lineEdit);

            label = new QLabel("g");
            lineEdit = new QLineEdit();
            lineEdit->setText(QString::number(color.y()));
            lineEdit->setValidator(new QDoubleValidator(0, 1, 3, widget));
            hLayout->addWidget(label);
            hLayout->addWidget(lineEdit);

            label = new QLabel("b");
            lineEdit = new QLineEdit();
            lineEdit->setText(QString::number(color.z()));
            lineEdit->setValidator(new QDoubleValidator(0, 1, 3, widget));
            hLayout->addWidget(label);
            hLayout->addWidget(lineEdit);

            label = new QLabel("a");
            lineEdit = new QLineEdit();
            lineEdit->setText(QString::number(color.w()));
            lineEdit->setValidator(new QDoubleValidator(0, 1, 3, widget));
            hLayout->addWidget(label);
            hLayout->addWidget(lineEdit);
            hLayout->setMargin(0);
            widget->setLayout(hLayout);
            tree->setItemWidget(item, 0, widget);
        }

        // circle radius
        if (shape->type == "Circle") {
            auto circle = (Circle *) shape;
            auto item = new QTreeWidgetItem(shapeParent);
            auto widget = new QWidget;
            auto label = new QLabel("radius");
            auto lineEdit = new QLineEdit;
            lineEdit->setValidator(new QDoubleValidator(0, 1000, 3, widget));
            lineEdit->setText(QString::number(circle->radius));
            auto hLayout = new QHBoxLayout;
            hLayout->setMargin(0);
            hLayout->addWidget(label);
            hLayout->addWidget(lineEdit);
            widget->setLayout(hLayout);
            tree->setItemWidget(item, 0, widget);
        }

        //rect width
        if (shape->type == "Rect") {
            Rect *rect = (Rect *) shape;
            auto item = new QTreeWidgetItem(shapeParent);
            auto widget = new QWidget;
            auto label = new QLabel("width");
            auto lineEdit = new QLineEdit;
            lineEdit->setValidator(new QDoubleValidator(0, 1000, 3, widget));
            lineEdit->setText(QString::number(rect->width));
            auto hLayout = new QHBoxLayout;
            hLayout->setMargin(0);
            hLayout->addWidget(label);
            hLayout->addWidget(lineEdit);
            widget->setLayout(hLayout);
            tree->setItemWidget(item, 0, widget);
        }

        // rect height
        if (shape->type == "Rect") {
            Rect *rect = (Rect *) shape;
            auto item = new QTreeWidgetItem(shapeParent);
            auto widget = new QWidget;
            auto label = new QLabel("height");
            auto lineEdit = new QLineEdit;
            lineEdit->setValidator(new QDoubleValidator(0, 1000, 3, widget));
            lineEdit->setText(QString::number(rect->height));
            auto hLayout = new QHBoxLayout;
            hLayout->setMargin(0);
            hLayout->addWidget(label);
            hLayout->addWidget(lineEdit);
            widget->setLayout(hLayout);
            tree->setItemWidget(item, 0, widget);
        }

        // position
        {
            auto item = new QTreeWidgetItem(shapeParent);
            item->setText(0, "position");
            item->setExpanded(true);

            item = new QTreeWidgetItem(item);

            auto widget = new QWidget;
            auto hLayout = new QHBoxLayout;

            auto label = new QLabel("x");
            auto lineEdit = new QLineEdit();
            lineEdit->setText(QString::number(shape->position.x()));
            lineEdit->setValidator(new QDoubleValidator(-1000, 1000, 3, widget));

            hLayout->addWidget(label);
            hLayout->addWidget(lineEdit);
            label = new QLabel("y");
            lineEdit = new QLineEdit();
            lineEdit->setText(QString::number(shape->position.y()));
            lineEdit->setValidator(new QDoubleValidator(-1000, 1000, 3, widget));

            hLayout->addWidget(label);
            hLayout->addWidget(lineEdit);

            hLayout->setMargin(0);
            widget->setLayout(hLayout);
            tree->setItemWidget(item, 0, widget);
        }
    }

    auto layout = new QHBoxLayout;
    layout->addWidget(tree);
    setLayout(layout);
}

QVector4D Settings::arrayToQColor(const QJsonArray &jsonArray)
{
    return {
            (float) jsonArray[0].toDouble(),
            (float) jsonArray[1].toDouble(),
            (float) jsonArray[2].toDouble(),
            (float) jsonArray[3].toDouble()
    };
}

void Settings::load()
{
    QFile loadFile("/Users/artich/dev/qCreativeTools/resources/settings.json");

    if (!loadFile.open(QIODevice::ReadOnly))
        qWarning("Couldn't open settings file.");

    QJsonDocument loadDoc = QJsonDocument::fromJson(loadFile.readAll());

    auto json = loadDoc.object();

     bgColor = arrayToQColor(json["bg_color"].toArray());
    qDebug() << bgColor;

    auto shapesArray = json["shapes"].toArray();

    for (const auto &shape : shapesArray) {
        auto shapeObject = shape.toObject();
        auto type = shapeObject["type"].toString();
        qDebug() << type;

        if (type == "Circle") {
            auto circle = new Circle;
            circle->type = type;
            circle->color = arrayToQColor(shapeObject["color"].toArray());
            circle->radius = (float) shapeObject["radius"].toDouble();
            circle->position = QVector2D((float) shapeObject["position"].toArray()[0].toDouble(),
                                         (float) shapeObject["position"].toArray()[1].toDouble());
            shapes.push_back(circle);
        } else if (type == "Rect") {
            auto rect = new Rect;
            rect->type = type;
            rect->color = arrayToQColor(shapeObject["color"].toArray());
            rect->width = (float) shapeObject["width"].toDouble();
            rect->height = (float) shapeObject["height"].toDouble();
            rect->position = QVector2D((float) shapeObject["position"].toArray()[0].toDouble(),
                                       (float) shapeObject["position"].toArray()[1].toDouble());
            shapes.push_back(rect);
        };
    }
}


