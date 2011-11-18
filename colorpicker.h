#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QWidget>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include <MDialog>

class ColorPicker : public MDialog
{
    Q_OBJECT
public:
    explicit ColorPicker(QGraphicsWidget *parent = 0);

signals:
    void colorSelected(QColor color);

public slots:
    void colorClicked(QColor color);

};

#endif // COLORPICKER_H
