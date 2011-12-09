#include <MLinearLayoutPolicy>
#include <QGraphicsLinearLayout>
#include <MLabel>
#include <MButton>
#include <MLayout>
#include <MToolBar>
#include <MAction>
#include <MTextEdit>
#include <MWidgetAction>
#include <MBanner>
#include <MSceneWindow>
#include <QDebug>
#include <MApplication>
#include <MOnDisplayChangeEvent>
#include <MApplicationWindow>

#include "mainpage.h"
#include "settingspage.h"
#include "linewidthdialog.h"
#include "toolpickerdialog.h"
#include "aboutpage.h"
#include "toolwidget.h"

MainPage::MainPage(QGraphicsItem *parent)
    : MApplicationPage(parent)
{

}

void MainPage::createContent()
{
    applicationWindow()->setStyleName("CommonApplicationWindowInverted");
    this->setStyleName("CommonApplicationPageInverted");

    MLayout *layout = new MLayout;
    centralWidget()->setLayout(layout);
    this->setPanningDirection(Qt::Horizontal | Qt::Vertical);
    this->setPannable(false);

    paintingArea = new PaintingArea(1024, 1024, this);

    MAction *menuOpenDialog = new MAction("Open", this);
    menuOpenDialog->setLocation(MAction::ApplicationMenuLocation);
    MAction *menuSaveDialog = new MAction("Save as", this);
    menuSaveDialog->setLocation(MAction::ApplicationMenuLocation);
    MAction *menuAboutDialog = new MAction("About", this);
    menuAboutDialog->setLocation(MAction::ApplicationMenuLocation);
    MAction *menuSettingsDialog = new MAction("Settings", this);
    menuSettingsDialog->setLocation(MAction::ApplicationMenuLocation);
    MAction *newImageMenu = new MAction("New image", this);
    newImageMenu->setLocation(MAction::ApplicationMenuLocation);

    this->addAction(menuAboutDialog);
    this->addAction(menuSettingsDialog);
    this->addAction(newImageMenu);
    this->addAction(menuSaveDialog);
    this->addAction(menuOpenDialog);

    MWidgetAction *rectangleAction = new MWidgetAction(this);
    QPixmap rectangleIcon(QString::fromUtf8("/opt/sketchit/data/sketchit_draw.png"));
    QPixmap rectangleIcon_dimmed(QString::fromUtf8("/opt/sketchit/data/sketchit_draw_dimmed.png"));
    ToolWidget *rectangleButton = new ToolWidget(0, rectangleIcon, rectangleIcon_dimmed);
    rectangleAction->setWidget(rectangleButton);
    rectangleAction->setLocation(MAction::ToolBarLocation);

    MAction *colorAction = new MAction("icon-m-image-edit-color", "Color", this);
    colorAction->setLocation(MAction::ToolBarLocation);

    MWidgetAction *lineAction = new MWidgetAction(this);
    QPixmap lineIcon(QString::fromUtf8("/opt/sketchit/data/sketchit_linewidth.png"));
    QPixmap lineIcon_dimmed(QString::fromUtf8("/opt/sketchit/data/sketchit_linewidth_dimmed.png"));
    ToolWidget *lineButton = new ToolWidget(0, lineIcon, lineIcon_dimmed);
    lineAction->setWidget(lineButton);
    lineAction->setLocation(MAction::ToolBarLocation);

    MWidgetAction *panningAction = new MWidgetAction(this);
    QPixmap panningIcon(QString::fromUtf8("/opt/sketchit/data/sketchit_pannable.png"));
    QPixmap panningIcon_dimmed(QString::fromUtf8("/opt/sketchit/data/sketchit_pannable_dimmed.png"));
    ToolWidget *panningButton = new ToolWidget(0, panningIcon, panningIcon_dimmed);
    panningAction->setWidget(panningButton);
    panningAction->setLocation(MAction::ToolBarLocation);

    colorWidget = new ColorCellWidget("black");
    MWidgetAction *colorWidgetAction = new MWidgetAction(this);
    colorWidgetAction->setLocation(MAction::ToolBarLocation);
    colorWidgetAction->setWidget(colorWidget);


    this->addAction(colorWidgetAction);
    this->addAction(rectangleAction);
    this->addAction(lineAction);
    this->addAction(panningAction);

    QGraphicsLinearLayout *paintArea = new QGraphicsLinearLayout(Qt::Vertical);

    paintArea->addItem(paintingArea);

    MLinearLayoutPolicy *landscapePolicy = new MLinearLayoutPolicy(layout, Qt::Horizontal);
    landscapePolicy->addItem(paintArea);
    layout->setLandscapePolicy(landscapePolicy);

    MLinearLayoutPolicy *portraitPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    portraitPolicy->addItem(paintArea);
    layout->setPortraitPolicy(portraitPolicy);

    connect(colorWidget, SIGNAL(signalClicked(QColor)), this, SLOT(showColorPicker()));
    connect(lineButton, SIGNAL(clicked(int)), this, SLOT(showLineWidthDialog()));
    connect(panningButton, SIGNAL(clicked(int)), this, SLOT(togglePanningMode()));
    connect(rectangleButton, SIGNAL(clicked(int)), this, SLOT(showToolPicker()));

    connect(menuOpenDialog, SIGNAL(triggered()), this, SLOT(showOpenDialog()));
    connect(menuSaveDialog, SIGNAL(triggered()), this, SLOT(showSaveDialog()));
    connect(menuSettingsDialog, SIGNAL(triggered()), this, SLOT(showSettingsPage()));
    connect(newImageMenu, SIGNAL(triggered()), this, SLOT(createNewImage()));
    connect(menuAboutDialog, SIGNAL(triggered()), this, SLOT(showAboutPage()));

    this->actualFileName = "";

    /* Auto-load functionality */
    if(getAutoLoad()) {
        QString filename = getAutoLoadFileName();
        if(!QString::compare(filename, "", Qt::CaseInsensitive) == 0)
            processOpenDialog(filename);
    }
}

void MainPage::createNewImage()
{
    /* Let's save the image, before creating a new. */
    decideAndSaveImage();
    paintingArea->createNewImage();
    this->setActualFileName("");
}

void MainPage::showAboutPage()
{
    AboutPage *aboutPage = new AboutPage();
    aboutPage->appear(scene(), MSceneWindow::DestroyWhenDismissed);
}

void MainPage::showSettingsPage()
{
    SettingsPage *settingsPage = new SettingsPage();
    settingsPage->appear(scene(), MSceneWindow::DestroyWhenDismissed);
}

void MainPage::showToolPicker()
{
    ToolPickerDialog *tp = new ToolPickerDialog();
    connect(tp, SIGNAL(toolTypeChanged(int)), this, SLOT(setTool(int)));
    tp->appear(MSceneWindow::DestroyWhenDismissed);
}

void MainPage::showColorPicker()
{
    ColorPicker *cp = new ColorPicker();
    connect(cp, SIGNAL(colorSelected(QColor)), this, SLOT(changeBrushColor(QColor)));
    connect(cp, SIGNAL(colorSelected(QColor)), colorWidget, SLOT(setColor(QColor)));
    cp->appear(MSceneWindow::DestroyWhenDismissed);
}

void MainPage::showOpenDialog()
{
    /* Let's save image before opening a new one. */
    decideAndSaveImage();
    OpenDialog *op = new OpenDialog();
    connect(op, SIGNAL(openImage(QString)), this, SLOT(processOpenDialog(QString)));
    op->appear(MSceneWindow::DestroyWhenDismissed);
}

void MainPage::showLineWidthDialog()
{
    LineWidthDialog *op = new LineWidthDialog();
    connect(op, SIGNAL(lineWidthChanged(int)), this, SLOT(changeLineWidth(int)));
    op->appear(MSceneWindow::DestroyWhenDismissed);
}

void MainPage::processOpenDialog(QString fileName)
{
    QString contains = APP_CATALOG;

    if(!fileName.contains(contains, Qt::CaseInsensitive))
        fileName = APP_CATALOG + fileName;

    QFile file(fileName);

    if (file.exists()) {
        this->paintingArea->openImage(file.fileName());
        this->setActualFileName(file.fileName());
        this->paintingArea->setIsImageModified(false);
    }
    else
        this->showWarningOpenFileBanner();
}

void MainPage::processSaveDialog(QString fileName)
{
    QString name = fileName;
    if(QString::compare(name, "", Qt::CaseInsensitive) == 0)
        name = this->actualFileName;

    this->paintingArea->saveImage(name);
    this->setActualFileName(name);
    this->paintingArea->setIsImageModified(false);
    this->showSavedBanner();
}

void MainPage::showSaveDialog()
{
    /* Let's save image before storing a copy. */
    decideAndSaveImage();
    SaveDialog *op = new SaveDialog(false, true);
    connect(op, SIGNAL(savedWithName(QString)), this, SLOT(processSaveDialog(QString)));
    op->appear(MSceneWindow::DestroyWhenDismissed);
}

void MainPage::showSaveDialogKnownFilename()
{
    /* Let's save image before storing a copy. */
    decideAndSaveImage();
    SaveDialog *op = new SaveDialog(true, false);
    connect(op, SIGNAL(savedWithName(QString)), this, SLOT(processSaveDialog(QString)));
    op->appear(MSceneWindow::DestroyWhenDismissed);
}

void MainPage::changeBrushColor(QColor color)
{
    paintingArea->setBrushColor(color);
}

void MainPage::setTool(int tool)
{
    paintingArea->setToolType(tool);
    if(tool == 5)
        paintingArea->setRubberMode(true);
    else
        paintingArea->setRubberMode(false);

}

void MainPage::togglePanningMode()
{
    if(this->isPannable()) {
        this->setPannable(false);
        paintingArea->setPanningMode(false);
    }
    else {
        this->setPannable(true);
        paintingArea->setPanningMode(true);
    }
    showPanningModeBanner();
}

void MainPage::setActualFileName(QString fileName)
{
    this->actualFileName = fileName;
}

QString MainPage::getActualFileName()
{
    if(QString::compare(this->actualFileName, "", Qt::CaseInsensitive) == 0) {
        showSaveDialog();
    }

    return this->actualFileName;
}

bool MainPage::saveWithDialog()
{
    if(QString::compare(this->actualFileName, "", Qt::CaseInsensitive) == 0)
        return true;
    else
        return false;
}

void MainPage::decideAndSaveImage()
{
    if (wantsToSaveFile()) {
        if(saveWithDialog())
            wantsToCloseWindow();
        else
            saveOnWindowEvents();
        storeAutoLoadFileName();
    }
}

/* Returns TRUE only when an image is modified and actual file name == null */
bool MainPage::wantsToSaveFile()
{
    return this->paintingArea->isImageModified();
}

void MainPage::saveOnWindowEvents()
{
    this->paintingArea->saveImage(this->actualFileName);
    this->paintingArea->setIsImageModified(false);
}

void MainPage::wantsToCloseWindow()
{
    QString filename = getNewFileName();
    this->setActualFileName(filename);
    this->paintingArea->saveImage(this->actualFileName);
    this->paintingArea->setIsImageModified(false);
}

void MainPage::setSaveNotifier(bool saved)
{
    if(!saved)
        notifyAction->setIconID("icon-m-common-teal");
    else
        notifyAction->setIconID("icon-m-common-red");
}
/*
void MainPage::showSaveNotifierBanner()
{
    MBanner *infoBanner = new MBanner();
    infoBanner->setStyleName(MBannerType::InformationBanner);
    if(this->paintingArea->isImageModified())
        infoBanner->setTitle("Image is modified, remember to save it");
    infoBanner->appear(scene(), MSceneWindow::DestroyWhenDone);
}
*/

void MainPage::showPanningModeBanner()
{
    MBanner *infoBanner = new MBanner();
    infoBanner->setStyleName(MBannerType::InformationBanner);
    if(this->isPannable())
        infoBanner->setTitle("Panning mode enabled.");
    else
        infoBanner->setTitle("Panning mode disabled.");
    infoBanner->appear(scene(), MSceneWindow::DestroyWhenDone);
}

void MainPage::showSavedBanner()
{
    MBanner *infoBanner = new MBanner();
    infoBanner->setStyleName(MBannerType::InformationBanner);
    infoBanner->setTitle("File saved.");
    infoBanner->appear(scene(), MSceneWindow::DestroyWhenDone);
}

void MainPage::showWarningOpenFileBanner()
{
    MBanner *infoBanner = new MBanner();
    infoBanner->setStyleName(MBannerType::InformationBanner);
    infoBanner->setTitle("Can't open the file.");
    infoBanner->appear(scene(), MSceneWindow::DestroyWhenDone);
}

void MainPage::changeLineWidth(int size)
{
    this->paintingArea->setLineWidth(size);
}

QString MainPage::getNewFileName()
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
            return baseName + ".png";
    }

    return baseName + ".png";
}

void MainPage::storeAutoLoadFileName()
{
    QSettings settings;

    settings.setValue("startup/autoload_filename", getActualFileName());
}

QString MainPage::getAutoLoadFileName()
{
    QSettings settings;
    QString value =  settings.value("startup/autoload_filename", "").toString();

    return value;
}

bool MainPage::getAutoLoad()
{
    QSettings settings;
    bool value =  settings.value("startup/autoload", true).toBool();

    return value;
}
