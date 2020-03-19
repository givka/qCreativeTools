#include "styled_item_delegate.h"
#include <QDebug>
#include <QPainter>
#include <QFileSystemModel>

void StyledItemDelegate::paint(QPainter *painter,
                               const QStyleOptionViewItem &option,
                               const QModelIndex &index) const
{
    painter->save();

    auto path = index.data(QFileSystemModel::FilePathRole).toString();
    //qDebug() << path;

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);

    QFont font = qvariant_cast<QFont>(index.data(Qt::FontRole));
    QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));

    QString filenameText = qvariant_cast<QString>(index.data(Qt::EditRole));

    QFontMetrics fontMetrics(font);
    QSize iconSize = icon.actualSize(option.decorationSize);

    QRect item = option.rect;

    QString elidedText = fontMetrics.elidedText(filenameText, Qt::ElideRight,
                                                item.width() - fontMetrics.maxWidth());

    auto pixmap = icon.pixmap(iconSize.width(), iconSize.height());

    const int x = item.center().x() - pixmap.width() / 2;
    const int y = item.center().y() - fontMetrics.height() - pixmap.height() / 2;

    QRect textRect = item;
    textRect.setTop(item.bottom() - 2 * fontMetrics.height());

    painter->setFont(font);

    if (option.state & QStyle::State_MouseOver) {
        pixmap = icon.pixmap(iconSize.width(), iconSize.height(), QIcon::Active);
        auto color = option.palette.highlight().color();
        color.setAlphaF(0.15);
        painter->fillRect(item, QBrush(color));
    }

    if (option.state & QStyle::State_Selected) {
        pixmap = icon.pixmap(iconSize.width(), iconSize.height(), QIcon::Selected);
        auto color = option.palette.highlight().color();
        color.setAlphaF(0.3);
        painter->fillRect(item, QBrush(color));
    }

    painter->drawPixmap(QRect(x, y, pixmap.width(), pixmap.height()), pixmap);

    painter->drawText(textRect, Qt::AlignCenter, elidedText);

    painter->restore();
}

QSize StyledItemDelegate::sizeHint(const QStyleOptionViewItem &option,
                                   const QModelIndex &index) const
{

    QFont font = qvariant_cast<QFont>(index.data(Qt::FontRole));
    QFontMetrics fontMetrics(font);

    return QSize(option.decorationSize) + 2 * QSize(fontMetrics.height(), fontMetrics.height());
}
