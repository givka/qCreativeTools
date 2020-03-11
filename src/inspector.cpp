#include "inspector.h"
#include "inspector_line.h"
#include <QMediaMetaData>
#include <QtMultimedia/QMediaPlayer>

#include <exif.h>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QScrollArea>

Inspector::Inspector(QWidget *parent) :
        QWidget(parent),
        m_mediaPlayer(new QMediaPlayer)
{
    lines = {
            new InspectorLine("Camera make", [](const easyexif::EXIFInfo &result) {
                return QString(" %1").arg(result.Make.c_str());
            }),
            new InspectorLine("Camera model", [](const easyexif::EXIFInfo &result) {
                return QString(" %1").arg(result.Model.c_str());
            }),
            new InspectorLine("Software", [](const easyexif::EXIFInfo &result) {
                return QString(" %1").arg(result.Software.c_str());
            }),
            new InspectorLine("Bits per sample", [](const easyexif::EXIFInfo &result) {
                return QString(" %1").arg(QString::number(result.BitsPerSample));
            }),
            new InspectorLine("Image width", [](const easyexif::EXIFInfo &result) {
                return QString(" %1").arg(QString::number(result.ImageWidth));
            }),
            new InspectorLine("Image height", [](const easyexif::EXIFInfo &result) {
                return QString(" %1").arg(QString::number(result.ImageHeight));
            }),
            new InspectorLine("Image description", [](const easyexif::EXIFInfo &result) {
                return QString(" %1").arg(result.ImageDescription.c_str());
            }),
            new InspectorLine("Image orientation", [](const easyexif::EXIFInfo &result) {
                return QString(" %1").arg(QString::number(result.Orientation));
            }),
            new InspectorLine("Image copyright", [](const easyexif::EXIFInfo &result) {
                return QString(" %1").arg(result.Copyright.c_str());
            }),
            new InspectorLine("Image date/time", [](const easyexif::EXIFInfo &result) {
                return QString(" %1").arg(result.DateTime.c_str());
            }),
            new InspectorLine("Original date/time", [](const easyexif::EXIFInfo &result) {
                return QString(" %1").arg(result.DateTimeOriginal.c_str());
            }),
            new InspectorLine("Digitize date/time", [](const easyexif::EXIFInfo &result) {
                return QString(" %1").arg(result.DateTimeDigitized.c_str());
            }),
            new InspectorLine("Subsecond time", [](const easyexif::EXIFInfo &result) {
                return QString(" %1").arg(result.SubSecTimeOriginal.c_str());
            }),
            new InspectorLine("Exposure time", [](const easyexif::EXIFInfo &result) {
                return QString(" 1/%1 s").arg(
                        QString::number((unsigned) (1.0 / result.ExposureTime)));
            }),
            new InspectorLine("F-stop", [](const easyexif::EXIFInfo &result) {
                return QString(" f/%1").arg(QString::number(result.FNumber, 'f', 1));
            }),
            new InspectorLine("Exposure program", [](const easyexif::EXIFInfo &result) {
                return QString(" %1").arg(QString::number(result.ExposureProgram));
            }),
            new InspectorLine("ISO speed", [](const easyexif::EXIFInfo &result) {
                return QString(" %1").arg(QString::number(result.ISOSpeedRatings));
            }),
            new InspectorLine("Subject distance", [](const easyexif::EXIFInfo &result) {
                return QString(" %1 m").arg(QString::number(result.SubjectDistance, 'f', 1));
            }),
            new InspectorLine("Exposure bias", [](const easyexif::EXIFInfo &result) {
                return QString(" %1 EV").arg(QString::number(result.ExposureBiasValue, 'f', 1));
            }),
            new InspectorLine("Flash used?", [](const easyexif::EXIFInfo &result) {
                return QString(" %1").arg(QString::number(result.Flash));
            }),
            new InspectorLine("Flash returned light", [](const easyexif::EXIFInfo &result) {
                return QString(" %1").arg(QString::number(result.FlashReturnedLight));
            }),
            new InspectorLine("Flash mode", [](const easyexif::EXIFInfo &result) {
                return QString(" %1").arg(QString::number(result.FlashMode));
            }),
            new InspectorLine("Metering mode", [](const easyexif::EXIFInfo &result) {
                return QString(" %1").arg(QString::number(result.MeteringMode));
            }),
            new InspectorLine("Lens focal length", [](const easyexif::EXIFInfo &result) {
                return QString(" %1 mm").arg(QString::number(result.FocalLength, 'f', 1));
            }),
            new InspectorLine("35mm focal length", [](const easyexif::EXIFInfo &result) {
                return QString(" %1 mm").arg(QString::number(result.FocalLengthIn35mm));
            }),
            new InspectorLine("GPS Latitude", [](const easyexif::EXIFInfo &result) {
                return QString(" %1 deg (%2 deg, %3 min, %4 sec %5)").arg(
                        QString::number(result.GeoLocation.Latitude, 'f', 1),
                        QString::number(result.GeoLocation.LatComponents.degrees, 'f', 1),
                        QString::number(result.GeoLocation.LatComponents.minutes, 'f', 1),
                        QString::number(result.GeoLocation.LatComponents.seconds, 'f', 1),
                        QString(result.GeoLocation.LatComponents.direction));
            }),
            new InspectorLine("GPS Longitude", [](const easyexif::EXIFInfo &result) {
                return QString(" %1 deg (%2 deg, %3 min, %4 sec %5)").arg(
                        QString::number(result.GeoLocation.Longitude, 'f', 1),
                        QString::number(result.GeoLocation.LonComponents.degrees, 'f', 1),
                        QString::number(result.GeoLocation.LonComponents.minutes, 'f', 1),
                        QString::number(result.GeoLocation.LonComponents.seconds, 'f', 1),
                        QString(result.GeoLocation.LonComponents.direction));
            }),
            new InspectorLine("GPS Altitude", [](const easyexif::EXIFInfo &result) {
                return QString(" %1 m").arg(QString::number(result.GeoLocation.Altitude, 'f', 1));
            }),
            new InspectorLine("GPS Precision (DOP)", [](const easyexif::EXIFInfo &result) {
                return QString(" %1").arg(QString::number(result.GeoLocation.DOP, 'f', 1));
            }),
            new InspectorLine("Lens min focal length", [](const easyexif::EXIFInfo &result) {
                return QString(" %1 mm").arg(
                        QString::number(result.LensInfo.FocalLengthMin, 'f', 1));
            }),
            new InspectorLine("Lens max focal length", [](const easyexif::EXIFInfo &result) {
                return QString(" %1 mm").arg(
                        QString::number(result.LensInfo.FocalLengthMax, 'f', 1));
            }),
            new InspectorLine("Lens f-stop min", [](const easyexif::EXIFInfo &result) {
                return QString(" f/%1").arg(QString::number(result.LensInfo.FStopMin, 'f', 1));
            }),
            new InspectorLine("Lens f-stop max", [](const easyexif::EXIFInfo &result) {
                return QString(" f/%1").arg(QString::number(result.LensInfo.FStopMax, 'f', 1));
            }),
            new InspectorLine("Lens make", [](const easyexif::EXIFInfo &result) {
                return QString(" %1").arg(result.LensInfo.Make.c_str());
            }),
            new InspectorLine("Lens model", [](const easyexif::EXIFInfo &result) {
                return QString(" %1").arg(result.LensInfo.Model.c_str());
            }),
            new InspectorLine("Focal plane XRes", [](const easyexif::EXIFInfo &result) {
                return QString(" %1").arg(
                        QString::number(result.LensInfo.FocalPlaneXResolution, 'f', 1));
            }),
            new InspectorLine("Focal plane YRes", [](const easyexif::EXIFInfo &result) {
                return QString(" %1").arg(
                        QString::number(result.LensInfo.FocalPlaneYResolution, 'f', 1));
            })
    };

    auto widget = new QWidget;
    auto vLayout = new QVBoxLayout;
    auto vLayout2 = new QVBoxLayout;

    for (const auto &line : lines) {
        vLayout->addWidget(line);
        /*  QFrame* l = new QFrame();
        l->setFrameShape(QFrame::HLine);
        l->setFrameShadow(QFrame::Sunken);
        vLayout->addWidget(l); */
    }
    widget->setLayout(vLayout);
    vLayout->setSpacing(0);

    auto scrollArea = new QScrollArea();
    vLayout2->addWidget(scrollArea);
    scrollArea->setWidget(widget);
    vLayout->setMargin(0);
    scrollArea->setFrameShape(QFrame::Box);
    scrollArea->setBackgroundRole(QPalette::Light);

    setLayout(vLayout2);
}

/* TODO: use this instead, but with the good format
    auto image = QImage(path);
    result.parseFrom(image.constBits(), image.sizeInBytes());
*/
void Inspector::setImage(const QString &path)
{
    FILE *fp = fopen(path.toStdString().c_str(), "rb");
    if (!fp){
        printf("Can't open file.\n");
        return;
    }

    fseek(fp, 0, SEEK_END);
    unsigned long fsize = ftell(fp);
    rewind(fp);
    auto buf = new unsigned char[fsize];

    if (fread(buf, 1, fsize, fp) != fsize) {
        printf("Can't read file.\n");
        delete[] buf;
        return;
    }

    fclose(fp);

    easyexif::EXIFInfo result;
    result.parseFrom(buf, fsize);

    for (auto &line: lines)
        line->updateLineEdit(result);

    delete[] buf;
}
