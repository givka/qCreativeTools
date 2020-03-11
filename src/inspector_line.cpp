#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QScrollArea>
#include "inspector_line.h"

InspectorLine::InspectorLine(const QString &labelName, QWidget *parent)
        : QWidget(parent),
          label(new QLabel(labelName)), lineEdit(new QLineEdit)
{
    lineEdit->setReadOnly(true);

    auto hLayout = new QHBoxLayout;
    hLayout->addWidget(label);
    hLayout->addWidget(lineEdit);
    hLayout->setMargin(4);
    setLayout(hLayout);
}
