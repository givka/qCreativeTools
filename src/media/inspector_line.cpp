#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QScrollArea>
#include <utility>
#include "inspector_line.h"

InspectorLine::InspectorLine(const QString &labelName,
                             std::function<QString(const easyexif::EXIFInfo &)> function,
                             QWidget *parent)
        : QWidget(parent),
          label(new QLabel(labelName)), function(std::move(function)), lineEdit(new QLineEdit)
{
    lineEdit->setReadOnly(true);

    auto hLayout = new QHBoxLayout;
    hLayout->addWidget(label);
    hLayout->addWidget(lineEdit);
    hLayout->setMargin(4);
    setLayout(hLayout);
}

void InspectorLine::setLine(const QString &line)
{
    lineEdit->setText(line);
}

void InspectorLine::updateLineEdit(const easyexif::EXIFInfo & result)
{
    lineEdit->setText(function(result));
}
