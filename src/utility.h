#pragma once

#include <QGroupBox>
#include <QLayout>

class Utility
{
public:
    static QGroupBox *createGroupBox(const QString &name, QLayout *layout,
                              const QList<QWidget *> &widgets);
};


