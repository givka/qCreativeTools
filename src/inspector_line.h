#pragma once

#include <QWidget>
#include <QLabel>
#include <QLineEdit>

class InspectorLine : public QWidget
{

public:
    explicit InspectorLine(const QString &labelName, QWidget *parent = nullptr);

private:
    QLabel *label;
    QLineEdit *lineEdit;
};


