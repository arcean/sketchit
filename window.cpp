#include <QDebug>
#include <QCloseEvent>
#include <MApplication>

#include "window.h"

void Window::closeEvent(QCloseEvent *event)
{
    if(mainpage->wantsToSaveFile()){
        if(mainpage->saveWithDialog()) {
            event->setAccepted(false);
            mainpage->wantsToCloseWindow();
            mainpage->storeAutoLoadFileName();
            MApplication::quit();
        }
        else {
            event->setAccepted(false);
            this->mainpage->saveOnWindowEvents();
            mainpage->storeAutoLoadFileName();
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
