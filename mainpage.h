#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <MApplicationPage>
#include <MDialog>
#include <QColor>
#include <MAction>

#include "paintingarea.h"
#include "colorpicker.h"
#include "opendialog.h"
#include "savedialog.h"
#include "toolpicker.h"
#include "colorcellwidget.h"

#define APP_NAME "SketchIt"
#define APP_CATALOG "/home/user/MyDocs/Pictures/SketchIt/"

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

private:
    void setActualFileName(QString fileName);
    QString getActualFileName();
    void showPanningModeBanner();
    void showSavedBanner();
    void showWarningOpenFileBanner();
    bool getAutoLoad();

    MDialog *colorDialog;
    PaintingArea *paintingArea;
    QString actualFileName;
    MAction *notifyAction;
    ColorCellWidget *colorWidget;

};

#endif // MAINPAGE_H
