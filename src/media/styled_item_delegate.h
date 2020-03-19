#pragma once

#include <QAbstractItemDelegate>
#include <QStyledItemDelegate>

class StyledItemDelegate : public QStyledItemDelegate
{
public:
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;
};


