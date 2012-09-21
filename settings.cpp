#include <QSettings>

#include "settings.h"

Settings::Settings(QObject *parent) :
    QObject(parent)
{
}

void Settings::setFullscreen(bool toggled)
{
    QSettings settings;

    settings.setValue("common/fullscreen", toggled);
}

void Settings::setAutoLoad(bool toggled)
{
    QSettings settings;

    settings.setValue("startup/autoload", toggled);
}

void Settings::setFeedback(bool toggled)
{
    QSettings settings;

    settings.setValue("common/feedback", toggled);
}

bool Settings::getFullscreen()
{
    QSettings settings;
    bool value =  settings.value("common/fullscreen", false).toBool();

    return value;
}

bool Settings::getAutoLoad()
{
    QSettings settings;
    bool value =  settings.value("startup/autoload", true).toBool();

    return value;
}

bool Settings::getFeedback()
{
    QSettings settings;
    bool value =  settings.value("common/feedback", true).toBool();

    return value;
}

void Settings::setImageSize(int value)
{
    QSettings settings;

    settings.setValue("image/size", value);
}

int Settings::getImageSize()
{
    // 0-low 1-medium 2-high
    QSettings settings;
    int value =  settings.value("image/size", 1).toInt();

    if(value > 2 || value < 0)
        value = 1;

    return value;
}

void Settings::setAutoLoadFileName(const QString &actualFileName)
{
    QSettings settings;

    settings.setValue("startup/autoload_filename", actualFileName);
}

QString Settings::getAutoLoadFileName()
{
    QSettings settings;
    QString value =  settings.value("startup/autoload_filename", "").toString();

    return value;
}
