#include "utility.h"

QGroupBox *Utility::createGroupBox(const QString &name, QLayout *layout,
                                   const QList<QWidget *> &widgets)
{
    for (const auto &widget : widgets)
        layout->addWidget(widget);

    auto groupBox = new QGroupBox(name);
    groupBox->setLayout(layout);
    return groupBox;
}
