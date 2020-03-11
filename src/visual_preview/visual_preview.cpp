#include <QtWidgets/QDesktopWidget>
#include "visual_preview.h"

VisualPreview::VisualPreview(QWidget *parent)
        : photos(new QCheckBox("Photos")),
          videos(new QCheckBox("Videos")),
          model(new QFileSystemModel),
          listView(new ListView),
          sizeIconSlider(new QSlider(Qt::Orientation::Horizontal))
{
    auto filterBox = new QGroupBox("Filters");
    auto hLayout = new QHBoxLayout;
    hLayout->addWidget(photos);
    hLayout->addWidget(videos);
    filterBox->setLayout(hLayout);

    connect(photos, &QCheckBox::stateChanged, this, &VisualPreview::filterStateChanged);
    connect(videos, &QCheckBox::stateChanged, this, &VisualPreview::filterStateChanged);
    connect(sizeIconSlider, &QSlider::valueChanged, this, [this](int value)
    {
        listView->setIconSize(QSize(value, value));
    });

    model->setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    model->setRootPath("C:/dev/myman/resources/");
    model->setIconProvider(new IconProvider);
    model->setNameFilterDisables(false);
    //model->setNameFilters(QStringList("*.png"));

    sizeIconSlider->setMinimum(32);
    sizeIconSlider->setMaximum(IconProvider::Size);
    sizeIconSlider->setValue(IconProvider::Size / 2);

    listView->setModel(model);
    listView->setViewMode(QListView::ViewMode::IconMode);
    listView->setResizeMode(QListView::ResizeMode::Adjust);
    listView->setIconSize(QSize(sizeIconSlider->value(), sizeIconSlider->value()));
    listView->setSpacing(11);
    listView->setMovement(QListView::Movement::Static);
    listView->setWordWrap(true);
    listView->setRootIndex(model->index(model->rootPath()));

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
        std::transform(IconProvider::ImageTypes.begin(), IconProvider::ImageTypes.end(), std::back_inserter(stringList),
                       [](const QString &string) { return "*." + string; });

    if (videos->isChecked())
        std::transform(IconProvider::VideoTypes.begin(), IconProvider::VideoTypes.end(), std::back_inserter(stringList),
                       [](const QString &string) { return "*." + string; });

    model->setNameFilters(stringList);

    qDebug() << stringList;
}

void VisualPreview::selectedChanged(const QModelIndex &current, const QModelIndex &previous)
{
    qDebug() << "VisualPreview::selectedChanged " << model->filePath(current);
    listView->setRootIndex(model->index(model->filePath(current)));
}