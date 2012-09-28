#include <MLayout>
#include <MGridLayoutPolicy>

#include "colorpicker.h"
#include "colorcell.h"

#define WIDTH 80
#define HEIGHT 80
#define WIDTH_LANDSCAPE 80
#define HEIGHT_LANDSCAPE 54
#define SPACING_LANDSCAPE 6
#define SPACING 6

ColorPicker::ColorPicker(int idToSelect, QGraphicsWidget *parent) :
    idToSelect(idToSelect)
{
    Q_UNUSED(parent);

    MWidget *centralColorWidget = new MWidget;
    QGraphicsLinearLayout *verticalLayout = new QGraphicsLinearLayout(Qt::Vertical, centralColorWidget);
    QGraphicsLinearLayout *horizontalLayout = new QGraphicsLinearLayout(Qt::Horizontal, verticalLayout);
    MLayout  *layout = new MLayout();
    QList<QColor> colors;
    colors << QColor(153, 255, 102); colors << QColor(102, 255, 51); colors << QColor(51, 204, 0); colors << QColor(0, 153, 0); colors << QColor(0, 102, 0);
    colors << QColor(153, 204, 255); colors << QColor(102, 153, 255); colors << QColor(51, 102, 255); colors << QColor(0, 51, 255); colors << QColor(0, 0, 204);
    colors << QColor(255, 153, 255); colors << QColor(255, 102, 255); colors << QColor(204, 51, 255); colors << QColor(153, 0, 204); colors << QColor(102, 0, 153);
    colors << QColor(255, 153, 153); colors << QColor(255, 102, 102); colors << QColor(255, 51, 51); colors << QColor(204, 0, 0); colors << QColor(153, 0, 0);
    colors << QColor(255, 255, 102); colors << QColor(255, 255, 0); colors << QColor(255, 204, 0); colors << QColor(255, 153, 0); colors << QColor(204, 102, 0);
    colors << QColor(204, 204, 204); colors << QColor(153, 153, 153); colors << QColor(102, 102, 102); colors << QColor(51, 51, 51); colors << QColor(0, 0, 0);

    this->setTitle("Choose a color");
    this->setModal(true);

    MGridLayoutPolicy *policy_portrait = new MGridLayoutPolicy(layout);
    MGridLayoutPolicy *policy_landscape = new MGridLayoutPolicy(layout);
    policy_portrait->setSpacing(SPACING);
    policy_landscape->setSpacing(SPACING_LANDSCAPE);
    policy_portrait->setContentsMargins(0, 24, 0, 0);
    policy_landscape->setContentsMargins(0, 0, 0, 0);
    layout->setContentsMargins(0, 0, 0, 0);

    bool select = false;

    for (int i = 0; i < CELLS_NUMER; ++i) {
        if (idToSelect == i)
            select = true;
        else
            select = false;

        cellsPortrait[i] = new ColorCell(colors.at(i), WIDTH, HEIGHT, select, i);
        cellsLandscape[i] = new ColorCell(colors.at(i), WIDTH, HEIGHT, select, i);
        policy_portrait->addItem(cellsPortrait[i], i / 5, i % 5, Qt::AlignCenter);
        policy_landscape->addItem(cellsLandscape[i], i % 5, i / 5, Qt::AlignCenter);
        connect(cellsPortrait[i], SIGNAL(signalClicked(QColor,int)), this, SLOT(colorClicked(QColor,int)));
        connect(cellsLandscape[i], SIGNAL(signalClicked(QColor,int)), this, SLOT(colorClicked(QColor,int)));
        connect(cellsPortrait[i], SIGNAL(signalPressed(int,bool)), this, SLOT(setToNormalState(int,bool)));
        connect(cellsLandscape[i], SIGNAL(signalPressed(int,bool)), this, SLOT(setToNormalState(int,bool)));
    }

    for (int i = 0; i < 6; i++) {
        policy_portrait->setRowFixedHeight(i, HEIGHT);
        policy_landscape->setColumnFixedWidth(i, WIDTH_LANDSCAPE);
    }
    for (int i = 0; i < 5; i++) {
        policy_portrait->setColumnFixedWidth(i, WIDTH);
        policy_landscape->setRowFixedHeight(i, HEIGHT_LANDSCAPE);
    }

    verticalLayout->addItem(horizontalLayout);
    verticalLayout->setAlignment(horizontalLayout, Qt::AlignCenter);
    horizontalLayout->addItem(layout);
    horizontalLayout->setAlignment(layout, Qt::AlignCenter);
    layout->setPortraitPolicy(policy_portrait);
    layout->setLandscapePolicy(policy_landscape);

    this->setCentralWidget(centralColorWidget);
}

ColorPicker::~ColorPicker()
{
    for (int i = 0; i < CELLS_NUMER; i++) {
        delete cellsPortrait[i];
        delete cellsLandscape[i];
    }
}

void ColorPicker::colorClicked(QColor color, int cellId)
{
    emit this->colorSelected(color, cellId);

    this->disappear();
}

/*
 * Perform 'hide' animation for all cells apart the cell with id = id.
 */
void ColorPicker::setToNormalState(int id, bool selectIdToSelect)
{
    for (int i = 0; i < CELLS_NUMER; i++) {
        if (i == id)
            continue;

        cellsPortrait[i]->setNormalState();
        cellsLandscape[i]->setNormalState();
    }

    if (selectIdToSelect) {
        cellsPortrait[idToSelect]->setSelectedState();
        cellsLandscape[idToSelect]->setSelectedState();
    }
}
