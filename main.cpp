#include <MApplication>
#include <MApplicationWindow>
#include <MApplicationPage>
#include <MButton>
#include <MToolBar>
#include <MAction>
#include <MWidgetAction>
#include <MTextEdit>
#include <QObject>

#include "mainpage.h"
#include "window.h"

int main(int argc, char *argv[]){
    MApplication application(argc, argv);
    application.setOrganizationName("arcean");
    application.setOrganizationDomain("arcean.com");
    application.setApplicationName("SketchIt");

    Window window;
    MainPage *mainPage = new MainPage();
    mainPage->connect(&window, SIGNAL(needToSave()), mainPage, SLOT(wantsToCloseWindow()));
    window.setMainPage(mainPage);
    mainPage->appear(&window);

    window.show();

    return application.exec();
 }
