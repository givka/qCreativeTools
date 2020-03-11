#include "scene.h"
#include <QDebug>
#include <QtWidgets/QVBoxLayout>

Scene::Scene(QWidget *parent)
        : QWidget(parent),
          imageLabel(new QLabel)
{
    imageLabel->setBackgroundRole(QPalette::Dark);
    auto vLayout = new QVBoxLayout;
    vLayout->addWidget(imageLabel);
    imageLabel->setAlignment(Qt::AlignCenter);
    setLayout(vLayout);
}

void Scene::setImage(const QString &path)
{
    auto pixMap = QPixmap(path);
    const int w = imageLabel->width();
    const int h = imageLabel->height();
    qDebug() << "Scene::setImage -> " << path << w << h;

    //.scaled(2*w, 2*h).scaled(w, h, Qt::TransformationMode::SmoothTransformation) is way faster
    imageLabel->setPixmap(pixMap.scaled(w, h, Qt::KeepAspectRatio, Qt::TransformationMode::SmoothTransformation));
}

