#include <MLinearLayoutPolicy>
#include <QGraphicsLinearLayout>
#include <MLabel>
#include <MButton>
#include <MLayout>
#include <MToolBar>
#include <MAction>
#include <MTextEdit>
#include <MSceneWindow>
#include <MApplication>
#include <MOnDisplayChangeEvent>
#include <MApplicationWindow>

#include "mainpage.h"
#include "settingspage.h"
#include "linewidthdialog.h"
#include "toolpickerdialog.h"
#include "aboutpage.h"
#include "toolwidget.h"
#include "Singleton.h"

MainPage::MainPage(QGraphicsItem *parent)
    : MApplicationPage(parent)
{

}

MainPage::~MainPage()
{

}

void MainPage::createContent()
{
    MTheme *theme = MTheme::instance();
    theme->loadCSS("/opt/sketchit/data/sketchit.css");
    theme->addPixmapDirectory("/opt/sketchit/data/");
    applicationWindow()->setStyleName("CommonApplicationWindowInverted");
    this->setStyleName("CommonApplicationPageInverted");
    applicationWindow()->setNavigationBarOpacity(0.9);

    MLayout *layout = new MLayout;
    centralWidget()->setLayout(layout);
    this->setPanningDirection(Qt::Horizontal | Qt::Vertical);
    this->setPannable(false);

    paintingArea = new PaintingArea(false, this);

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
    menuUndo = new MAction("Undo", this);
    menuUndo->setLocation(MAction::NoLocation);
    menuRedo = new MAction("Redo", this);
    menuRedo->setLocation(MAction::NoLocation);

    this->addAction(menuAboutDialog);
    this->addAction(menuSettingsDialog);
    this->addAction(newImageMenu);
    this->addAction(menuSaveDialog);
    this->addAction(menuOpenDialog);
    this->addAction(menuRedo);
    this->addAction(menuUndo);

    MAction *rectangleAction = new MAction("sketchit_draw", "Draw", this);
  //  MWidgetAction *rectangleAction = new MWidgetAction(this);
  //  QPixmap rectangleIcon(QString::fromUtf8("/opt/sketchit/data/sketchit_draw.png"));
  //  QPixmap rectangleIcon_dimmed(QString::fromUtf8("/opt/sketchit/data/sketchit_draw_dimmed.png"));
  //  ToolWidget *rectangleButton = new ToolWidget(0, rectangleIcon, rectangleIcon_dimmed, false, NULL, NULL);
  //  rectangleAction->setWidget(rectangleButton);
    rectangleAction->setLocation(MAction::ToolBarLocation);

    MAction *colorAction = new MAction("icon-m-image-edit-color", "Color", this);
    colorAction->setLocation(MAction::ToolBarLocation);

    lineAction = new MAction("sketchit_linewidth", "Line", this);
   // lineAction = new MWidgetAction(this);
   // QPixmap lineIcon(QString::fromUtf8("/opt/sketchit/data/sketchit_linewidth.png"));
   // QPixmap lineIcon_dimmed(QString::fromUtf8("/opt/sketchit/data/sketchit_linewidth_dimmed.png"));
  //  ToolWidget *lineButton = new ToolWidget(0, lineIcon, lineIcon_dimmed, false, NULL, NULL);
  //  lineAction->setWidget(lineButton);
    lineAction->setLocation(MAction::NoLocation);

    panningAction = new MAction("sketchit_pannable", "Line", this);
   // QPixmap panningIcon(QString::fromUtf8("/opt/sketchit/data/sketchit_pannable.png"));
   // QPixmap panningIcon_dimmed(QString::fromUtf8("/opt/sketchit/data/sketchit_pannable_dimmed.png"));
   // QPixmap panningIcon_selected(QString::fromUtf8("/opt/sketchit/data/sketchit_pannable_selected.png"));
   // QPixmap panningIcon_selected_dimmed(QString::fromUtf8("/opt/sketchit/data/sketchit_pannable_selected_dimmed.png"));
   // ToolWidget *panningButton = new ToolWidget(0, panningIcon, panningIcon_dimmed, true, panningIcon_selected, panningIcon_selected_dimmed);
   // panningAction->setWidget(panningButton);
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
    connect(lineAction, SIGNAL(triggered()), this, SLOT(showLineWidthDialog()));
    connect(panningAction, SIGNAL(triggered()), this, SLOT(togglePanningMode()));
    connect(rectangleAction, SIGNAL(triggered()), this, SLOT(showToolPicker()));

    connect(menuOpenDialog, SIGNAL(triggered()), this, SLOT(showOpenDialog()));
    connect(menuSaveDialog, SIGNAL(triggered()), this, SLOT(showSaveDialog()));
    connect(menuSettingsDialog, SIGNAL(triggered()), this, SLOT(showSettingsPage()));
    connect(newImageMenu, SIGNAL(triggered()), this, SLOT(createNewImage()));
    connect(menuUndo, SIGNAL(triggered()), paintingArea, SLOT(undoPop()));
    connect(menuRedo, SIGNAL(triggered()), paintingArea, SLOT(redoPop()));
    connect(menuAboutDialog, SIGNAL(triggered()), this, SLOT(showAboutPage()));

    connect(paintingArea, SIGNAL(countUndo(int)), this, SLOT(undoAction(int)));
    connect(paintingArea, SIGNAL(countRedo(int)), this, SLOT(redoAction(int)));

    /* Initialize Settings. */
    settings = &Singleton<Settings>::Instance();

    /* Initialize banner. */
    infoBanner = new MBanner();
    showInfoBanner = false;

    connect (infoBanner, SIGNAL(displayExited()), this, SLOT(showPanningModeBannerOnDisappear()));

    this->actualFileName = "";

    /* Auto-load functionality */
    bool newImageNeeded = true;

    if(settings->getAutoLoad()) {
        QString filename = settings->getAutoLoadFileName();
        if(!QString::compare(filename, "", Qt::CaseInsensitive) == 0)
            newImageNeeded = !processOpenDialog(filename);
    }

    if (newImageNeeded)
        paintingArea->createNewImage();

    /* Haptic feedback functionality */
    paintingArea->setFeedbackEnabled(settings->getFeedback());

    /* Fullscreen mode */
    setFullscreenMode();

    /* Other inits */
    // By default select cell with black color.
    colorCellNum = 29;
}

void MainPage::setFullscreenMode()
{
    MWindow *window = MApplication::activeWindow();

    if (!window)
        return;

    if (settings->getFullscreen()) {
        window->showFullScreen();
    }
    else {
        window->showNormal();
    }
}

void MainPage::changePaintingAreaSettings()
{
    paintingArea->setFeedbackEnabled(settings->getFeedback());
}

void MainPage::undoAction(int count_undo)
{
    if (count_undo == 0)
        menuUndo->setLocation(MAction::NoLocation);
    else
        menuUndo->setLocation(MAction::ApplicationMenuLocation);
}

void MainPage::redoAction(int count_redo)
{
    if (count_redo == 0)
        menuRedo->setLocation(MAction::NoLocation);
    else
        menuRedo->setLocation(MAction::ApplicationMenuLocation);
}

void MainPage::createNewImage()
{
    /* Let's save the image, before creating a new. */
    decideAndSaveImage();
    paintingArea->createNewImage();
    this->setActualFileName("");
    paintingArea->resetUndoRedoCounters();
}

void MainPage::showAboutPage()
{
    AboutPage *aboutPage = new AboutPage();
    aboutPage->appear(scene(), MSceneWindow::DestroyWhenDismissed);
}

void MainPage::showSettingsPage()
{
    SettingsPage *settingsPage = new SettingsPage();
    connect(settingsPage, SIGNAL(settingsChanged()), this, SLOT(changePaintingAreaSettings()));
    settingsPage->appear(scene(), MSceneWindow::DestroyWhenDismissed);
}

void MainPage::showToolPicker()
{
    ToolPickerDialog *tp = new ToolPickerDialog(paintingArea->getToolType());
    connect(tp, SIGNAL(toolTypeChanged(int)), this, SLOT(setTool(int)));
    tp->appear(MSceneWindow::DestroyWhenDismissed);
}

void MainPage::showColorPicker()
{
    ColorPicker *cp = new ColorPicker(colorCellNum);
    connect(cp, SIGNAL(colorSelected(QColor, int)), this, SLOT(parseColor(QColor,int)));

    cp->appear(MSceneWindow::DestroyWhenDismissed);
}

void MainPage::parseColor(QColor color, int cellId)
{
    paintingArea->setBrushColor(color);
    colorWidget->setColor(color);
    colorCellNum = cellId;
}

void MainPage::showOpenDialog()
{
    /* Let's save image before opening a new one. */
    decideAndSaveImage();
    OpenDialog *op = new OpenDialog(this, getActualFileName());
    connect(op, SIGNAL(openImage(QString)), this, SLOT(processOpenDialog(QString)));
    op->appear(MSceneWindow::DestroyWhenDismissed);
}

void MainPage::showLineWidthDialog()
{
    LineWidthDialog *op = new LineWidthDialog(paintingArea->getLineWidth());
    connect(op, SIGNAL(lineWidthChanged(int)), paintingArea, SLOT(setLineWidth(int)));
    op->appear(MSceneWindow::DestroyWhenDismissed);
}

bool MainPage::processOpenDialog(QString fileName)
{
    QString contains = APP_CATALOG;
    bool ret = false;

    if(!fileName.contains(contains, Qt::CaseInsensitive))
        fileName = APP_CATALOG + fileName;

    QFile file(fileName);

    if (file.exists()) {
        this->paintingArea->openImage(file.fileName());
        this->setActualFileName(file.fileName());
        ret = true;
    }
    //else
    //    this->showWarningOpenFileBanner();

    return ret;
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

void MainPage::setTool(int tool)
{
    paintingArea->setToolType(tool);

    if (tool == 5)
        paintingArea->setRubberMode(true);
    else
        paintingArea->setRubberMode(false);

    if (tool == 1 || tool == 2)
        lineAction->setLocation(MAction::ToolBarLocation);
    else
        lineAction->setLocation(MAction::NoLocation);
}

void MainPage::togglePanningMode()
{
    if(this->isPannable()) {
        panningAction->setIconID("sketchit_pannable");
        this->setPannable(false);
        paintingArea->setPanningMode(false);
    }
    else {
        panningAction->setIconID("sketchit_pannable_selected");
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
        return getNewFileName();
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
        settings->setAutoLoadFileName(getActualFileName());
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
    showInfoBanner = true;

    if (infoBanner->isOnDisplay())
        infoBanner->dismiss();
    else
        showPanningModeBannerOnDisappear();
}

void MainPage::showPanningModeBannerOnDisappear()
{
    if (showInfoBanner) {
        infoBanner->setStyleName(MBannerType::SystemBanner);
        if(this->isPannable()) {
            infoBanner->setIconID("sketchit_pannable_selected");
            infoBanner->setTitle("Panning mode enabled");
        }
        else {
            infoBanner->setIconID("sketchit_pannable");
            infoBanner->setTitle("Panning mode disabled");
        }
        showInfoBanner = false;
        infoBanner->appear(scene(), MSceneWindow::KeepWhenDone);
    }
}

void MainPage::showSavedBanner()
{
    infoBanner->setStyleName(MBannerType::InformationBanner);
    infoBanner->setTitle("File saved");
    infoBanner->appear(scene(), MSceneWindow::KeepWhenDone);
}

void MainPage::showWarningOpenFileBanner()
{
    infoBanner->setStyleName(MBannerType::InformationBanner);
    infoBanner->setTitle("Can't open the file");
    infoBanner->appear(scene(), MSceneWindow::KeepWhenDone);
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

        if(counter > 500000)
            return baseName + ".png";
    }

    return baseName + ".png";
}
