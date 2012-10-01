#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

//#define ENABLE_SHAKE

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = 0);

    void setFullscreen(bool toggled);
    void setAutoLoad(bool toggled);
    void setFeedback(bool toggled);
    void setImageSize(int value);
    bool getAutoLoad();
    bool getFeedback();
    int getImageSize();
    bool getFullscreen();

#ifdef ENABLE_SHAKE
    void setShakeColorPicker(bool toggled);
    bool getShakeColorPicker();
#endif

    void setAutoLoadFileName(const QString &actualFileName);
    QString getAutoLoadFileName();
    
signals:
    
public slots:
    
};

#endif // SETTINGS_H
