#pragma once

#include <QtWidgets/QWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QFileSystemModel>
#include <QListView>
#include <QDebug>
#include "../icon_provider.h"
#include "../list_view.h"

class VisualPreview : public QWidget
{
Q_OBJECT

public:
    explicit VisualPreview(QWidget *parent = nullptr);

    ListView *getListView() { return listView; }

private :
    ListView *listView;
    QCheckBox *videos;
    QCheckBox *photos;
    QSlider *sizeIconSlider;

public slots:

    void selectedChanged(const QModelIndex &current, const QModelIndex &previous);

private slots:

    void filterStateChanged();
};


