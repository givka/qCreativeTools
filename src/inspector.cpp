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
    connect(m_mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &Inspector::mediaStatusChanged);

    QVector<InspectorLine *> lines =
            {
                    new InspectorLine("Camera make:"),
                    new InspectorLine("Camera model:"),
                    new InspectorLine("Software:"),
                    new InspectorLine("Bits per sample:"),
                    new InspectorLine("Image width:"),
                    new InspectorLine("Image height:"),
                    new InspectorLine("Image description:"),
                    new InspectorLine("Image orientation:"),
                    new InspectorLine("Image copyright:"),
                    new InspectorLine("Image date/time:"),
                    new InspectorLine("Original date/time:"),
                    new InspectorLine("Digitize date/time:"),
                    new InspectorLine("Subsecond time:"),
                    new InspectorLine("Exposure time:"),
                    new InspectorLine("F-stop:"),
                    new InspectorLine("Exposure program:"),
                    new InspectorLine("ISO speed:"),
                    new InspectorLine("Subject distance:"),
                    new InspectorLine("Exposure bias:"),
                    new InspectorLine("Flash used?:"),
                    new InspectorLine("Flash returned light:"),
                    new InspectorLine("Flash mode:"),
                    new InspectorLine("Metering mode:"),
                    new InspectorLine("Lens focal length:"),
                    new InspectorLine("35mm focal length:"),
                    new InspectorLine("GPS Latitude:"),
                    new InspectorLine("GPS Longitude:"),
                    new InspectorLine("GPS Altitude:"),
                    new InspectorLine("GPS Precision (DOP):"),
                    new InspectorLine("Lens min focal length:"),
                    new InspectorLine("Lens max focal length:"),
                    new InspectorLine("Lens f-stop min:"),
                    new InspectorLine("Lens f-stop max:"),
                    new InspectorLine("Lens make:"),
                    new InspectorLine("Lens model:"),
                    new InspectorLine("Focal plane XRes:"),
                    new InspectorLine("Focal plane YRes:")
            };

    qDebug() << layout();
    auto widget = new QWidget;
    auto vLayout = new QVBoxLayout;
    auto vLayout2 = new QVBoxLayout;
    for (const auto &line : lines) {
        vLayout->addWidget(line);
        /*
        QFrame* l = new QFrame();
        l->setFrameShape(QFrame::HLine);
        l->setFrameShadow(QFrame::Sunken);
        vLayout->addWidget(l);
        */
    }
    widget->setLayout(vLayout);
    vLayout->setSpacing(0);

    auto scrollArea = new QScrollArea();
    vLayout2->addWidget(scrollArea);
    scrollArea->setWidget(widget);

    setLayout(vLayout2);
}

void Inspector::setImage(const QString &path)
{
    qDebug() << "Inspector::setImage" << path;

    m_mediaPlayer->setMedia(QUrl::fromLocalFile(path));

    // Read the JPEG file into a buffer
    FILE *fp = fopen(path.toStdString().c_str(), "rb");
    if (!fp) {
        printf("Can't open file.\n");
    }
    fseek(fp, 0, SEEK_END);
    unsigned long fsize = ftell(fp);
    rewind(fp);
    auto buf = new unsigned char[fsize];
    if (fread(buf, 1, fsize, fp) != fsize) {
        printf("Can't read file.\n");
        delete[] buf;
    }

    fclose(fp);

    easyexif::EXIFInfo result;
    result.parseFrom(buf, fsize);

    /* TODO: use this instead, but with the good format
    auto image = QImage(path);
    result.parseFrom(image.constBits(), image.sizeInBytes());
    */

    // Dump EXIF information
    printf("Camera make          : %s\n", result.Make.c_str());
    printf("Camera model         : %s\n", result.Model.c_str());
    printf("Software             : %s\n", result.Software.c_str());
    printf("Bits per sample      : %d\n", result.BitsPerSample);
    printf("Image width          : %d\n", result.ImageWidth);
    printf("Image height         : %d\n", result.ImageHeight);
    printf("Image description    : %s\n", result.ImageDescription.c_str());
    printf("Image orientation    : %d\n", result.Orientation);
    printf("Image copyright      : %s\n", result.Copyright.c_str());
    printf("Image date/time      : %s\n", result.DateTime.c_str());
    printf("Original date/time   : %s\n", result.DateTimeOriginal.c_str());
    printf("Digitize date/time   : %s\n", result.DateTimeDigitized.c_str());
    printf("Subsecond time       : %s\n", result.SubSecTimeOriginal.c_str());
    printf("Exposure time        : 1/%d s\n",
           (unsigned) (1.0 / result.ExposureTime));
    printf("F-stop               : f/%.1f\n", result.FNumber);
    printf("Exposure program     : %d\n", result.ExposureProgram);
    printf("ISO speed            : %d\n", result.ISOSpeedRatings);
    printf("Subject distance     : %f m\n", result.SubjectDistance);
    printf("Exposure bias        : %f EV\n", result.ExposureBiasValue);
    printf("Flash used?          : %d\n", result.Flash);
    printf("Flash returned light : %d\n", result.FlashReturnedLight);
    printf("Flash mode           : %d\n", result.FlashMode);
    printf("Metering mode        : %d\n", result.MeteringMode);
    printf("Lens focal length    : %f mm\n", result.FocalLength);
    printf("35mm focal length    : %u mm\n", result.FocalLengthIn35mm);
    printf("GPS Latitude         : %f deg (%f deg, %f min, %f sec %c)\n",
           result.GeoLocation.Latitude, result.GeoLocation.LatComponents.degrees,
           result.GeoLocation.LatComponents.minutes,
           result.GeoLocation.LatComponents.seconds,
           result.GeoLocation.LatComponents.direction);
    printf("GPS Longitude        : %f deg (%f deg, %f min, %f sec %c)\n",
           result.GeoLocation.Longitude, result.GeoLocation.LonComponents.degrees,
           result.GeoLocation.LonComponents.minutes,
           result.GeoLocation.LonComponents.seconds,
           result.GeoLocation.LonComponents.direction);
    printf("GPS Altitude         : %f m\n", result.GeoLocation.Altitude);
    printf("GPS Precision (DOP)  : %f\n", result.GeoLocation.DOP);
    printf("Lens min focal length: %f mm\n", result.LensInfo.FocalLengthMin);
    printf("Lens max focal length: %f mm\n", result.LensInfo.FocalLengthMax);
    printf("Lens f-stop min      : f/%.1f\n", result.LensInfo.FStopMin);
    printf("Lens f-stop max      : f/%.1f\n", result.LensInfo.FStopMax);
    printf("Lens make            : %s\n", result.LensInfo.Make.c_str());
    printf("Lens model           : %s\n", result.LensInfo.Model.c_str());
    printf("Focal plane XRes     : %f\n", result.LensInfo.FocalPlaneXResolution);
    printf("Focal plane YRes     : %f\n", result.LensInfo.FocalPlaneYResolution);

    delete[] buf;
}

//TODO: test exif from github
void Inspector::mediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    qDebug() << "mediaStatusChanged" << status;
    QStringList metadatalist = m_mediaPlayer->availableMetaData();

    int list_size = metadatalist.size();

    qDebug() << m_mediaPlayer->isMetaDataAvailable() << list_size;

    QString metadata_key;
    QVariant var_data;

    for (int indx = 0; indx < list_size; indx++) {
        // Get the key from the list
        metadata_key = metadatalist.at(indx);

        // Get the value for the key
        var_data = m_mediaPlayer->metaData(metadata_key);

        qDebug() << metadata_key << var_data.toString();
    }
}



