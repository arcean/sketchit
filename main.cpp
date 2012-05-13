#include <MApplication>
#include <MApplicationWindow>
#include <MApplicationPage>
#include <MButton>
#include <MToolBar>
#include <MAction>
#include <MWidgetAction>
#include <MTextEdit>
#include <QObject>
#include <MComponentCache>

#include "mainpage.h"
#include "window.h"

M_EXPORT int main(int argc, char *argv[]){
    MApplication* application = MComponentCache::mApplication(argc, argv);
    application->setOrganizationName("arcean");
    application->setOrganizationDomain("arcean.com");
    application->setApplicationName("SketchIt");

    MApplicationWindow* window = MComponentCache::mApplicationWindow();
    MainPage *mainPage = new MainPage();
    mainPage->connect(window, SIGNAL(needToSave()), mainPage, SLOT(wantsToCloseWindow()));

    mainPage->appear(window);

    window->show();

    return application->exec();
 }
