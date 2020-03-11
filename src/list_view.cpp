#include "list_view.h"
#include "icon_provider.h"
#include "styled_item_delegate.h"
#include <QDebug>

ListView::ListView()
        : QListView(),
          model(new QFileSystemModel)
{
    model->setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    model->setRootPath("C:/dev/qCreativeTools/resources/");
    model->setIconProvider(new IconProvider);
    model->setNameFilterDisables(false);
    //model->setNameFilters(QStringList("*.png"));

    connect(this, &QAbstractItemView::doubleClicked, this, &ListView::navigateTo);
}

void ListView::init()
{
    setModel(model);
    setViewMode(QListView::ViewMode::IconMode);
    setResizeMode(QListView::ResizeMode::Adjust);
    setIconSize(QSize(IconProvider::Size / 2, IconProvider::Size / 2));
    setSpacing(11);
    setMovement(QListView::Movement::Static);
    setWordWrap(true);
    setRootIndex(model->index(model->rootPath()));
    setItemDelegate(new StyledItemDelegate);

}

void ListView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    auto info = model->fileInfo(current);
    auto suffix = info.suffix().toLower();

    if (IconProvider::ImageTypes.contains(suffix) || IconProvider::VideoTypes.contains(suffix)) {
        qDebug() << "emit showPreview";
        emit showPreview(info.filePath());
    }

    QListView::currentChanged(current, previous);
}

void ListView::navigateTo(const QModelIndex &index)
{
    auto info = model->fileInfo(index);
    if (info.isDir()) {
        setRootIndex(model->index(info.filePath()));
        emit updateTree(info.filePath());
    }
}
