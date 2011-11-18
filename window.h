#ifndef WINDOW_H
#define WINDOW_H

#include <QObject>
#include <MApplicationWindow>

#include "mainpage.h"

class Window : public MApplicationWindow
{
    Q_OBJECT
protected:
    virtual void closeEvent(QCloseEvent *event);

signals:
    void needToSave();

public:
    void setSaveFileFlag(bool set);
    bool getSaveFileFlag();
    void setMainPage(MainPage *mainPage);

public slots:

private:
    bool saveFileFlag;
    MainPage *mainpage;

};

#endif // WINDOW_H
