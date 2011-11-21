#ifndef SAVEDIALOG_H
#define SAVEDIALOG_H

#include <QWidget>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include <MSheet>
#include <MBasicSheetHeader>
#include <MTextEdit>

class SaveDialog : public MSheet
{
    Q_OBJECT
public:
    explicit SaveDialog(bool exitApp, bool saveAs, QGraphicsWidget *parent = 0);

private:
    QString getNewFileName();
    MTextEdit *textEdit;
    bool exitApp;
    bool saveAs;

private slots:
    void processDialogResult();
    void processDialogRejected();

signals:
    void savedWithName(QString filename);

};

#endif // SAVEDIALOG_H
