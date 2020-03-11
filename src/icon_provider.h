#pragma once

#include <QFileIconProvider>
#include <QSet>

class IconProvider : public QFileIconProvider
{
public:

    IconProvider() : QFileIconProvider() {};

    QIcon icon(const QFileInfo &info) const override;

    static constexpr int Size = 128;

    static const QStringList ImageTypes;
    static const QStringList VideoTypes;
};


