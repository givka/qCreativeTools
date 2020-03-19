#pragma once

# include <QSortFilterProxyModel>
# include <QDebug>

class FilterModel : public QSortFilterProxyModel
{

Q_OBJECT

public:
    explicit FilterModel(QObject *parent = nullptr);

    void setFilters(const QStringList &list) { filters = list; }

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QStringList filters{};
};


