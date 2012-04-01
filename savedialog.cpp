#include <MLayout>
#include <MGridLayoutPolicy>
#include <MLabel>
#include <MLinearLayoutPolicy>
#include <MButton>
#include <QDir>
#include <QFile>
#include <MApplication>
#include <MSeparator>

#include "savedialog.h"
#include "mainpage.h"

SaveDialog::SaveDialog(bool exitApp, bool saveAs, QGraphicsWidget *parent)
{
    this->setStyleName("Inverted");

    /* Sheet header */
    MBasicSheetHeader *sheetHeader = new MBasicSheetHeader;

 /*   QGraphicsLinearLayout *headerLayout = new QGraphicsLinearLayout(this->headerWidget());
    MButton *negativeButton = new MButton("Cancel");
    MButton *positiveButton = new MButton("Save");
    negativeButton->setObjectName("CommonSheetHeaderButtonInverted");
    positiveButton->setObjectName("CommonSheetHeaderButtonAccentInverted");

    headerLayout->addItem(negativeButton);
    headerLayout->addStretch();
    headerLayout->addItem(positiveButton);

    connect(negativeButton, SIGNAL(clicked()), SLOT(processDialogRejected()));
    connect(positiveButton, SIGNAL(clicked()), SLOT(processDialogResult()));*/

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
        label = new MLabel("Save as:");
    else
        label = new MLabel("Do you want to save changes?");

    if(saveAs) {
        textEdit = new MTextEdit(MTextEditModel::SingleLine,
                                                 QString());
        QString data = getNewFileName();
        data.chop(4);
        textEdit->setText(data);
    }
    label->setObjectName("CommonBodyTextInvertedBig");

    MLabel *labelExtension = new MLabel("The file extension is <b>.png</b>");
    labelExtension->setObjectName("CommonBodyTextInverted");
    MSeparator *separator = new MSeparator();

    landscapePolicy->addItem(label);
    portraitPolicy->addItem(label);
    if(saveAs) {
        landscapePolicy->addItem(separator);
        portraitPolicy->addItem(separator);
        landscapePolicy->addItem(textEdit);
        portraitPolicy->addItem(textEdit);
        landscapePolicy->addItem(labelExtension);
        portraitPolicy->addItem(labelExtension);
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
        filename.append(".png");


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

        if(counter > 500000)
            ready = true;
    }

    return file.fileName();
}

