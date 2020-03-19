#pragma once

#include <QtWidgets>
#include <QtConcurrent/QtConcurrent>

class IconProxy : public QSortFilterProxyModel
{
Q_OBJECT

public:

    void setFilters(const QStringList &list);

    explicit IconProxy(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;

signals:

    void hasIcon(const QString &, const QIcon &, const QPersistentModelIndex &index) const;

private slots:

    void onIcon(const QString &path, const QIcon &icon, const QPersistentModelIndex &index);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QMap<QString, QIcon> icons;
    QStringList filters{};

    QIcon getIcon(const QString &path) const;
};

