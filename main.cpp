#include <MApplication>
#include <MApplicationWindow>
#include <MApplicationPage>
#include <MButton>
#include <MToolBar>
#include <MAction>
#include <MWidgetAction>
#include <MTextEdit>
#include <QObject>
#include <MExport>
#include <MComponentCache>

#include "mainpage.h"
#include "window.h"

M_EXPORT int main(int argc, char *argv[]){
    MApplication* application = MComponentCache::mApplication(argc, argv);

    application->setOrganizationName("arcean");
    application->setOrganizationDomain("arcean.com");
    application->setApplicationName("SketchIt");

    Window window;
    MainPage mainPage;
    mainPage.connect(&window, SIGNAL(needToSave()), &mainPage, SLOT(wantsToCloseWindow()));

    window.setMainPage(&mainPage);
    mainPage.appear(&window);
    window.show();

    return application->exec();
 }
