#pragma once

#include <QTreeView>
#include <QVector2D>
#include <QVector4D>
#include "shape.h"

class Settings : public QWidget
{
Q_OBJECT

public :
    Settings();

signals:

    void render(const QVector4D &bgColor, const QVector<Shape *> &shapes);

private:
    QVector<Shape *> shapes{};

    void load();

    static QVector4D arrayToQColor(const QJsonArray &jsonArray);

    QVector4D bgColor;

    class MyDoubleValidator : public QDoubleValidator{
    public:
        explicit MyDoubleValidator(QObject* parent= nullptr)
                :QDoubleValidator(parent){}
        MyDoubleValidator(double bottom, double top, int decimals, QObject* parent=nullptr)
                :QDoubleValidator(bottom, top, decimals, parent){}

        virtual void fixup(QString& input)const override{
            input= QString::number(bottom(), 'f', decimals());
        }
    };
};


