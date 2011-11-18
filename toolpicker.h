#ifndef TOOLPICKER_H
#define TOOLPICKER_H

#include <QWidget>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include <MDialog>
#include <MTextEdit>

class ToolPicker : public MDialog
{
    Q_OBJECT
public:
    explicit ToolPicker(QGraphicsWidget *parent = 0);

private:
    MTextEdit *textEdit;

private slots:
    void toolSelected(int tool);

signals:
    void selectedTool(int tool);

};

#endif // TOOLPICKER_H
