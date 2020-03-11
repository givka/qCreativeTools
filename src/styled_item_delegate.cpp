#include "styled_item_delegate.h"
#include <QDebug>
#include <QPainter>
#include <QFileSystemModel>

void StyledItemDelegate::paint(QPainter *painter,
                               const QStyleOptionViewItem &option,
                               const QModelIndex &index) const
{
    auto model = qvariant_cast<QFileSystemModel *>(index.model());
    qDebug() << "painting..." << model;

    painter->save();

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




    /*
    painter->save();

    QFont font = qvariant_cast<QFont>(index.data(Qt::FontRole));
    font.setPixelSize(10);
    QFontMetrics fontMetrics(font);

    QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
    QSize iconsize = icon.actualSize(option.decorationSize);

    QString filenameText = qvariant_cast<QString>(index.data(Qt::EditRole));

    QRect item = option.rect;
    QRect iconRect(item.left()+4, item.top()+4, iconsize.width(), iconsize.height());
    QPainterPath iconPath;
    iconPath.addRoundRect(iconRect, 12, 12);

    QRect textRect = item;
    textRect.setTop(item.top()+item.height()-fontMetrics.height()-10);
    QPainterPath textPath;
    textPath.addRoundedRect(textRect, 8, 8);

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);

    painter->setFont(font);
//    painter->fillPath(textPath, Qt::gray);
//    painter->drawRoundedRect(textRect, 6, 6);
    painter->drawText(textRect, Qt::AlignCenter, filenameText);

    painter->setPen(Qt::darkGray);
    painter->setClipping(true);
    painter->setClipPath(iconPath);
    painter->drawPixmap(iconRect, icon.pixmap(iconsize.width(),iconsize.height()));
    painter->setClipping(false);

    painter->setPen(Qt::darkGray);
    painter->drawPath(iconPath);

    painter->setPen(Qt::darkGray);
    if (option.state & QStyle::State_Selected) {
        QPen pen(Qt::white);
        pen.setWidth(2);
        painter->setPen(pen);
    }
    painter->drawRoundedRect(item, 8, 8);

    painter->restore();

     */
}

QSize StyledItemDelegate::sizeHint(const QStyleOptionViewItem &option,
                                   const QModelIndex &index) const
{

    QFont font = qvariant_cast<QFont>(index.data(Qt::FontRole));
    QFontMetrics fontMetrics(font);

    return QSize(option.decorationSize) + 2 * QSize(fontMetrics.height(), fontMetrics.height());
}
