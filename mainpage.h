#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <MApplicationPage>
#include <MDialog>
#include <QColor>
#include <MAction>
#include <MWidgetAction>
#include <MBanner>

#include "paintingarea.h"
#include "colorpicker.h"
#include "opendialog.h"
#include "savedialog.h"
#include "toolpicker.h"
#include "colorcellwidget.h"

#define APP_NAME "SketchIt"
#define APP_CATALOG "/home/user/MyDocs/Pictures/SketchIt/"

/* To enable debug msgs: */
/* -output-level debug */

class MainPage : public MApplicationPage {
    Q_OBJECT
public:
    MainPage(QGraphicsItem *parent = 0);

    bool saveWithDialog();
    void saveOnWindowEvents();
    QString getNewFileName();
    void decideAndSaveImage();
    void storeAutoLoadFileName();
    QString getAutoLoadFileName();
    QString getActualFileName();

protected:
    virtual void createContent();

public slots:
    void wantsToCloseWindow();
    bool wantsToSaveFile();
    void processSaveDialog(QString fileName);
    void showSaveDialog();
    void showSaveDialogKnownFilename();

private slots:
    void showSettingsPage();
    void changeBrushColor(QColor color);
    void togglePanningMode();
    void showColorPicker();
    void showToolPicker();
    void setTool(int tool);
    void showOpenDialog();
    void processOpenDialog(QString fileName);
    void setSaveNotifier(bool saved);
    void changeLineWidth(int size);
    void showLineWidthDialog();
    void createNewImage();
    void showAboutPage();
    void undoAction(int count_undo);
    void redoAction(int count_redo);
    void changePaintingAreaSettings();

private:
    void setActualFileName(QString fileName);
    void showPanningModeBanner();
    void showSavedBanner();
    void showWarningOpenFileBanner();
    bool getAutoLoad();
    bool getFeedback();
    bool getFullscreenMode();
    void setFullscreenMode();

    MDialog *colorDialog;
    PaintingArea *paintingArea;
    QString actualFileName;
    MAction *notifyAction;
    ColorCellWidget *colorWidget;
    MAction *lineAction;
    MAction *panningAction;
    MAction *menuRedo;
    MAction *menuUndo;
    MBanner *infoBanner;

};

#endif // MAINPAGE_H
