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
    MWidget *centralWidget = new MWidget;
    MLayout *layout = new MLayout();
    layout->setContentsMargins(0,0,0,0);

    MLinearLayoutPolicy* landscapePolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    MLinearLayoutPolicy* portraitPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);

    this->exitApp = exitApp;
    this->saveAs = saveAs;

    MLabel *label;
    if(saveAs)
        label = new MLabel("File name:");
    else
        label = new MLabel("Do you want to save changes?");

    if(saveAs) {
        textEdit = new MTextEdit(MTextEditModel::SingleLine,
                                                 QString());
        textEdit->setText(getNewFileName());
    }

    landscapePolicy->addItem(label);
    portraitPolicy->addItem(label);
    label->setColor("white");
    if(saveAs) {
        landscapePolicy->addItem(textEdit);
        portraitPolicy->addItem(textEdit);
    }

    layout->setPortraitPolicy(portraitPolicy);
    layout->setLandscapePolicy(landscapePolicy);

    centralWidget->setLayout(layout);
    this->setCentralWidget(centralWidget);
    this->setModal(true);
    if(saveAs)
        this->setTitle("Save as...");
    else
        this->setTitle("Save...");

    addButton("Save", M::ActionRole);
    if(exitApp)
        addButton(M::CloseButton);

    connect(this, SIGNAL(accepted()), SLOT(processDialogResult()));
    if(exitApp)
        connect(this, SIGNAL(rejected()), SLOT(processDialogRejected()));
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
}

void SaveDialog::processDialogRejected()
{

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

