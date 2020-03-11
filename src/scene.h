#pragma once

#include <QWidget>
#include <QLabel>

class Scene : public QWidget
{
Q_OBJECT

public :

    explicit Scene(QWidget *parent = nullptr);

public slots:

    void setImage(const QString &path);

private :
    QLabel *imageLabel;
};


