#include <MLayout>
#include <MGridLayoutPolicy>
#include <QDebug>

#include "colorpicker.h"
#include "colorcell.h"

#define WIDTH 80
#define HEIGHT 80
#define WIDTH_LANDSCAPE 82
#define HEIGHT_LANDSCAPE 56

ColorPicker::ColorPicker(QGraphicsWidget *parent)
{
    MWidget *centralColorWidget = new MWidget;
    MLayout  *layout = new MLayout();
    QList<QColor> colors;

    colors << QColor(153, 255, 102); colors << QColor(102, 255, 51); colors << QColor(51, 204, 0); colors << QColor(0, 153, 0); colors << QColor(0, 102, 0);
    colors << QColor(153, 204, 255); colors << QColor(102, 153, 255); colors << QColor(51, 102, 255); colors << QColor(0, 51, 255); colors << QColor(0, 0, 204);
    colors << QColor(255, 153, 255); colors << QColor(255, 102, 255); colors << QColor(204, 51, 255); colors << QColor(153, 0, 204); colors << QColor(102, 0, 153);
    colors << QColor(255, 153, 153); colors << QColor(255, 102, 102); colors << QColor(255, 51, 51); colors << QColor(204, 0, 0); colors << QColor(153, 0, 0);
    colors << QColor(255, 255, 102); colors << QColor(255, 255, 0); colors << QColor(255, 204, 0); colors << QColor(255, 153, 0); colors << QColor(204, 102, 0);
    colors << QColor(204, 204, 204); colors << QColor(153, 153, 153); colors << QColor(102, 102, 102); colors << QColor(51, 51, 51); colors << QColor(0, 0, 0);

    this->setTitle("Select color");
    centralColorWidget->setLayout(layout);
    this->setModal(true);

    MGridLayoutPolicy *policy_portrait = new MGridLayoutPolicy(layout);
    MGridLayoutPolicy *policy_landscape = new MGridLayoutPolicy(layout);
    policy_portrait->setSpacing(6);
    policy_landscape->setSpacing(7);
    policy_portrait->setContentsMargins(14, 0, 14, 0);
    policy_landscape->setContentsMargins(14, 0, 14, 0);

    for (int i = 0; i < 30; ++i) {
        ColorCell *cell_portrait = new ColorCell(colors.at(i), WIDTH, HEIGHT);
        ColorCell *cell_landscape = new ColorCell(colors.at(29-i), WIDTH, HEIGHT);
        policy_portrait->addItem(cell_portrait, i / 5, i % 5, Qt::AlignCenter);
        policy_landscape->addItem(cell_landscape, i % 5, i / 5, Qt::AlignCenter);
        connect(cell_portrait, SIGNAL(signalClicked(QColor)), this, SLOT(colorClicked(QColor)));
        connect(cell_landscape, SIGNAL(signalClicked(QColor)), this, SLOT(colorClicked(QColor)));
    }

    for (int i = 0; i < 6; i++) {
        policy_portrait->setRowFixedHeight(i, HEIGHT);
        policy_landscape->setColumnFixedWidth(i, WIDTH_LANDSCAPE);
    }
    for (int i = 0; i < 5; i++) {
        policy_portrait->setColumnFixedWidth(i, WIDTH);
        policy_landscape->setRowFixedHeight(i, HEIGHT_LANDSCAPE);
    }

    layout->setPortraitPolicy(policy_portrait);
    layout->setLandscapePolicy(policy_landscape);

    this->setCentralWidget(centralColorWidget);
}

void ColorPicker::colorClicked(QColor color)
{
    emit this->colorSelected(color);

    this->disappear();
}
