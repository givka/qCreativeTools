#include "list_view.h"
#include "icon_provider.h"
#include "styled_item_delegate.h"
#include "icon_proxy.h"

ListView::ListView()
        : QListView(),
          model(new QFileSystemModel),
          filterModel(new FilterModel),
          iconProxy(new IconProxy)
{
    model->setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    model->setRootPath("C:/dev/qCreativeTools/resources/");
    model->setIconProvider(new IconProvider);
    model->setNameFilterDisables(false);
    iconProxy->setSourceModel(model);
    iconProxy->setFilterRegExp(QRegExp("", Qt::CaseInsensitive, QRegExp::FixedString));

    connect(this, &QAbstractItemView::doubleClicked, this, &ListView::navigateTo);
}

void ListView::init()
{
    setModel(iconProxy);
    setRootIndex(iconProxy->mapFromSource(model->index(model->rootPath())));
    setViewMode(QListView::ViewMode::IconMode);
    setResizeMode(QListView::ResizeMode::Adjust);
    setIconSize(QSize(IconProvider::Size / 2, IconProvider::Size / 2));
    setSpacing(11);
    setMovement(QListView::Movement::Static);
    setWordWrap(true);
    setItemDelegate(new StyledItemDelegate);
}

/*
void ListView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    qDebug()<<"currentChanged";
    qDebug()<<model;
    auto info = model->fileInfo(current);
    qDebug()<<info;


    auto suffix = info.suffix().toLower();
    qDebug()<<"currentChanged";

    if (IconProvider::ImageTypes.contains(suffix) || IconProvider::VideoTypes.contains(suffix)) {
        qDebug() << "emit showPreview";
        emit showPreview(info.filePath());
    }

    QListView::currentChanged(current, previous);
}
*/

void ListView::navigateTo(const QModelIndex &index)
{
    auto info = model->fileInfo((iconProxy->mapToSource(index)));
    if (info.isDir()) {
        setRootIndex(iconProxy->mapFromSource(model->index(info.filePath())));
        //emit updateTree(info.filePath());
    }
}

void ListView::setFilters(const QStringList &list)
{
    auto j = list.join("|");
    iconProxy->setFilterRegExp(QRegExp(j, Qt::CaseInsensitive, QRegExp::FixedString));
    iconProxy->setFilters(list);
}
