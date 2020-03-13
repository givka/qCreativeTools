#include "list_view.h"
#include "icon_provider.h"
#include "styled_item_delegate.h"
#include "icon_proxy.h"

ListView::ListView()
        : QListView(),
          model(new QFileSystemModel),
          filterModel(new FilterModel)
// ,iconProxy(new IconProxy)
{
    model->setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);

#if __APPLE__
    model->setRootPath("/Users/artich/dev/qCreativeTools/resources/");
#elif _WIN32
    model->setRootPath("C:/dev/qCreativeTools/resources/");
#endif

    model->setIconProvider(new IconProvider);
    model->setNameFilterDisables(false);
    //iconProxy->setSourceModel(model);
    //iconProxy->setFilterRegExp(QRegExp("", Qt::CaseInsensitive, QRegExp::FixedString));

    connect(this, &QAbstractItemView::doubleClicked, this, &ListView::navigateTo);
}

void ListView::init()
{
    setModel(model);
    setRootIndex(/*iconProxy->mapFromSource*/(model->index(model->rootPath())));
    setViewMode(QListView::ViewMode::IconMode);
    setResizeMode(QListView::ResizeMode::Adjust);
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
    auto info = model->fileInfo(/*(iconProxy->mapToSource(index))*/index);
    if (info.isDir()) {
        setRootIndex(/*iconProxy->mapFromSource(model->index(info.filePath()))*/
                model->index(info.filePath()));
        emit updateTree(info.filePath());
    }
}

void ListView::setFilters(const QStringList &list)
{
    QStringList filters;
    qDebug() << list;
    std::transform(list.begin(), list.end(),
                   std::back_inserter(filters),
                   [](const QString &string) { return "*." + string; });

    qDebug() << filters;

    model->setNameFilters(filters);
}
