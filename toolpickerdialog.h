#ifndef TOOLPICKERDIALOG_H
#define TOOLPICKERDIALOG_H

#include <QWidget>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include <MDialog>
#include <MTextEdit>
#include <MList>

class ToolPickerDialog : public MDialog
{
    Q_OBJECT
public:
    explicit ToolPickerDialog(int toolItem, QGraphicsWidget *parent = 0);

private slots:
    void itemClicked(const QModelIndex &index);

signals:
    void toolTypeChanged(int size);

private:
    MList *list;
};

#endif // TOOLPICKERDIALOG_H
