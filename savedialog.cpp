#include <MLayout>
#include <MGridLayoutPolicy>
#include <MLabel>
#include <MLinearLayoutPolicy>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <MApplication>

#include "savedialog.h"
#include "mainpage.h"

SaveDialog::SaveDialog(bool exitApp, bool saveAs, QGraphicsWidget *parent)
{
    this->setStyleName("Inverted");

    /* Sheet header */
    MBasicSheetHeader *sheetHeader = new MBasicSheetHeader;
    sheetHeader->setStyleName("Inverted");

    sheetHeader->setPositiveAction(new QAction("Save", sheetHeader));
    connect(sheetHeader->positiveAction(), SIGNAL(triggered()), SLOT(processDialogResult()));

    sheetHeader->setNegativeAction(new QAction("Cancel", sheetHeader));
    connect(sheetHeader->negativeAction(), SIGNAL(triggered()), SLOT(processDialogRejected()));

    setHeaderWidget(sheetHeader);
    /* ============ */

    MWidget *centralWidget = new MWidget;
    MLayout *layout = new MLayout();

    MLinearLayoutPolicy* landscapePolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    MLinearLayoutPolicy* portraitPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    landscapePolicy->setSpacing(8);
    portraitPolicy->setSpacing(8);

    this->exitApp = exitApp;
    this->saveAs = saveAs;

    MLabel *label;
    if(saveAs)
        label = new MLabel("Save file as:");
    else
        label = new MLabel("Do you want to save changes?");

    if(saveAs) {
        textEdit = new MTextEdit(MTextEditModel::SingleLine,
                                                 QString());
        textEdit->setText(getNewFileName());
    }
    label->setColor("white");

    landscapePolicy->addItem(label);
    portraitPolicy->addItem(label);
    if(saveAs) {
        landscapePolicy->addItem(textEdit);
        portraitPolicy->addItem(textEdit);
    }

    layout->setPortraitPolicy(portraitPolicy);
    layout->setLandscapePolicy(landscapePolicy);
    layout->setMaximumHeight(100);

    centralWidget->setLayout(layout);
    this->setCentralWidget(centralWidget);
}

void SaveDialog::processDialogResult()
{
    if(saveAs) {
        QString filename = textEdit->text();


        if(QString::compare(filename, "", Qt::CaseInsensitive) == 0) {
            filename = getNewFileName();
        }
        emit savedWithName(APP_CATALOG + filename);
    }
    else
        emit savedWithName("");
    this->close();
}

void SaveDialog::processDialogRejected()
{
    this->close();
}

QString SaveDialog::getNewFileName()
{
    QDir appDir(APP_CATALOG);
    bool ready = false;
    int counter = 0;
    QString baseName = "sketchit";

    if(!appDir.exists()) {
        appDir.mkdir(APP_CATALOG);
    }

    QDir::setCurrent(APP_CATALOG);
    QFile file;

    while (!ready) {
        if(counter > 0)
            file.setFileName(baseName + "(" + QString::number(counter) +").png");
        else
            file.setFileName(baseName + ".png");

        if(!file.exists()) {
            return file.fileName();
        }

        counter++;

        if(counter > 50000)
            ready = true;
    }
}

