#include <QDebug>
#include <QCloseEvent>
#include <MApplication>

#include "window.h"

void Window::closeEvent(QCloseEvent *event)
{
    //if(getSaveFileFlag()) {
    if(mainpage->wantsToSaveFile()){
        if(mainpage->saveWithDialog()) {
            //qDebug() << "Trying to prevent closing our window...";
            event->setAccepted(false);
            //  this->enterDisplayEvent();
            //  this->show();
           // this->activateWindow();
            // this->showNormal();
           // emit this->needToSave();
            mainpage->wantsToCloseWindow();
            mainpage->storeAutoLoadFileName();
            MApplication::quit();
        }
        else {
            event->setAccepted(false);
            //this->activateWindow();
            this->mainpage->saveOnWindowEvents();
            mainpage->storeAutoLoadFileName();
            MApplication::quit();
        }
    }
    else {
        //qDebug() << "Trying to close window...";
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
