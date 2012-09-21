#ifndef LINEWIDTHDIALOG_H
#define LINEWIDTHDIALOG_H

#include <QWidget>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include <MDialog>
#include <MTextEdit>
#include <MList>

class LineWidthDialog : public MDialog
{
    Q_OBJECT
public:
    explicit LineWidthDialog(int lineWidth, QGraphicsWidget *parent = 0);

private slots:
    void itemClicked(const QModelIndex &index);

signals:
    void lineWidthChanged(int size);

private:
    MList *list;
};

#endif // LINEWIDTHDIALOG_H
