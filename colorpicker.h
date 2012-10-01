#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QWidget>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include <MDialog>

#include "colorcell.h"

#define CELLS_NUMER 30

class ColorPicker : public MDialog
{
    Q_OBJECT
public:
    explicit ColorPicker(int idToSelect = -1, QGraphicsWidget *parent = 0);
    ~ColorPicker();

signals:
    void colorSelected(QColor color, int cellId);

public slots:

private slots:
    void colorClicked(QColor color, int cellId);
    void setToNormalState(int id, bool selectIdToSelect);

private:
    ColorCell *cellsPortrait[CELLS_NUMER];
    ColorCell *cellsLandscape[CELLS_NUMER];

    int idToSelect;

};

#endif // COLORPICKER_H
