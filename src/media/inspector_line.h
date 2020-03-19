#pragma once

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <exif.h>

class InspectorLine : public QWidget
{
Q_OBJECT

public:
    explicit InspectorLine(const QString &labelName,
                           std::function<QString(const easyexif::EXIFInfo &)> function,
                           QWidget *parent = nullptr);

    void setLine(const QString &line);

    void updateLineEdit(const easyexif::EXIFInfo & result);

private:
    QLabel *label;
    QLineEdit *lineEdit;
    std::function<QString(const easyexif::EXIFInfo &)> function;
};


