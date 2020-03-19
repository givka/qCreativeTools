#include <QtWidgets/QDesktopWidget>
#include "visual_preview.h"

VisualPreview::VisualPreview(QWidget *parent)
        : photos(new QCheckBox("Photos")),
          videos(new QCheckBox("Videos")),
          listView(new ListView),
          sizeIconSlider(new QSlider(Qt::Orientation::Horizontal))
{
    listView->init();

    auto filterBox = new QGroupBox("Filters");
    auto hLayout = new QHBoxLayout;
    hLayout->addWidget(photos);
    hLayout->addWidget(videos);
    filterBox->setLayout(hLayout);

    connect(photos, &QCheckBox::stateChanged, this, &VisualPreview::filterStateChanged);
    connect(videos, &QCheckBox::stateChanged, this, &VisualPreview::filterStateChanged);
    connect(sizeIconSlider, &QSlider::valueChanged, this, [this](int value) {
        listView->setIconSize(QSize(value, value));
    });

    sizeIconSlider->setMinimum(32);
    sizeIconSlider->setMaximum(IconProvider::Size);
    sizeIconSlider->setValue((int) (IconProvider::Size / 2.5));

    auto sliderBox = new QGroupBox("Icon size");
    sliderBox->setLayout(new QVBoxLayout);
    sliderBox->layout()->addWidget(sizeIconSlider);

    auto vLayout = new QVBoxLayout;
    vLayout->addWidget(filterBox);
    vLayout->addWidget(listView);
    vLayout->addWidget(sliderBox);

    setLayout(vLayout);
}

void VisualPreview::filterStateChanged()
{
    auto stringList = QStringList();

    if (photos->isChecked())
        stringList.append(IconProvider::ImageTypes);
    if (videos->isChecked())
        stringList.append(IconProvider::VideoTypes);

    listView->setFilters(stringList);
}
