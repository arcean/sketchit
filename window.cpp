#include <QDebug>
#include <QCloseEvent>
#include <MApplication>

#include "window.h"
#include "settings.h"
#include "Singleton.h"

void Window::closeEvent(QCloseEvent *event)
{
    Settings *settings = &Singleton<Settings>::Instance();

    if(mainpage->wantsToSaveFile()){
        if(mainpage->saveWithDialog()) {
            event->setAccepted(false);
            mainpage->wantsToCloseWindow();
            settings->setAutoLoadFileName(mainpage->getActualFileName());
            MApplication::quit();
        }
        else {
            event->setAccepted(false);
            this->mainpage->saveOnWindowEvents();
            settings->setAutoLoadFileName(mainpage->getActualFileName());
            MApplication::quit();
        }
    }
    else {
        event->setAccepted(true);
    }
}

void Window::setSaveFileFlag(bool set)
{
    this->saveFileFlag = set;
}

bool Window::getSaveFileFlag()
{
    return this->saveFileFlag;
}

void Window::setMainPage(MainPage *mainPage)
{
    this->mainpage = mainPage;
}
