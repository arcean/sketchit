#include <MLayout>
#include <MGridLayoutPolicy>
#include <MLabel>
#include <MLinearLayoutPolicy>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QDebug>

#include "toolpicker.h"
#include "toolwidget.h"

#define WIDTH 90
#define HEIGHT 80
#define WIDTH_LANDSCAPE 74
#define HEIGHT_LANDSCAPE 56

ToolPicker::ToolPicker(QGraphicsWidget *parent)
{
    MWidget *centralWidget = new MWidget;
    MLayout  *layout = new MLayout();

    this->setTitle("Select tool");
    centralWidget->setLayout(layout);
    this->setModal(true);

    MGridLayoutPolicy *policy_portrait = new MGridLayoutPolicy(layout);
    MGridLayoutPolicy *policy_landscape = new MGridLayoutPolicy(layout);
    policy_portrait->setSpacing(80);
    policy_landscape->setSpacing(16);

    ToolWidget *fingerButton = new ToolWidget(0, "icon-l-gallery");
  //  fingerButton->setViewType(ToolWidget::iconType);
 //   fingerButton->setIconID("icon-l-gallery");
    ToolWidget *rectangleButton = new ToolWidget(2, "icon-l-gallery");
  //  rectangleButton->setViewType(ToolWidget::iconType);
  //  rectangleButton->setIconID("icon-l-gallery");
    ToolWidget *elipseButton = new ToolWidget(3, "icon-l-gallery");
 //   elipseButton->setViewType(ToolWidget::iconType);
  //  elipseButton->setIconID("icon-l-gallery");
    ToolWidget *lineButton = new ToolWidget(4, "icon-l-gallery");
  //  lineButton->setViewType(ToolWidget::iconType);
 //   lineButton->setIconID("icon-l-gallery");
    ToolWidget *penButton = new ToolWidget(1, "icon-l-gallery");
  //  penButton->setViewType(ToolWidget::iconType);
 //   penButton->setIconID("icon-l-gigfinder");
   // penButton->setGeometry(QRectF(0, 0, WIDTH, HEIGHT));
    ToolWidget *rubberButton = new ToolWidget(5, "icon-l-gallery");
  //  rubberButton->setViewType(ToolWidget::iconType);
 //   rubberButton->setIconID("icon-l-gallery");

    policy_portrait->addItem(fingerButton, 0, 0, Qt::AlignCenter);
    policy_portrait->addItem(rectangleButton, 0, 1, Qt::AlignCenter);
    policy_portrait->addItem(elipseButton, 0, 2, Qt::AlignCenter);
    policy_portrait->addItem(lineButton, 1, 0, Qt::AlignCenter);
    policy_portrait->addItem(penButton, 1, 1, Qt::AlignCenter);
    policy_portrait->addItem(rubberButton, 1, 2, Qt::AlignCenter);
    policy_landscape->addItem(fingerButton, 0, 0, Qt::AlignCenter);
    policy_landscape->addItem(rectangleButton, 0, 1, Qt::AlignCenter);
    policy_landscape->addItem(elipseButton, 0, 2, Qt::AlignCenter);
    policy_landscape->addItem(lineButton, 0, 3, Qt::AlignCenter);
    policy_landscape->addItem(penButton, 0, 4, Qt::AlignCenter);
    policy_landscape->addItem(rubberButton, 0, 5, Qt::AlignCenter);

    connect(fingerButton, SIGNAL(clicked(int)), this, SLOT(toolSelected(int)));
    connect(rectangleButton, SIGNAL(clicked(int)), this, SLOT(toolSelected(int)));
    connect(elipseButton, SIGNAL(clicked(int)), this, SLOT(toolSelected(int)));
    connect(lineButton, SIGNAL(clicked(int)), this, SLOT(toolSelected(int)));
    connect(penButton, SIGNAL(clicked(int)), this, SLOT(toolSelected(int)));
    connect(rubberButton, SIGNAL(clicked(int)), this, SLOT(toolSelected(int)));

    for (int i = 0; i < 3; i++) {
        policy_portrait->setColumnFixedWidth(i, WIDTH);
    }
    policy_portrait->setRowFixedHeight(0, HEIGHT);
    policy_portrait->setRowFixedHeight(1, HEIGHT);

    for (int i = 0; i < 6; i++) {
        policy_landscape->setColumnFixedWidth(i, WIDTH_LANDSCAPE);
    }
    policy_landscape->setRowFixedHeight(0, HEIGHT);

    layout->setPortraitPolicy(policy_portrait);
    layout->setLandscapePolicy(policy_landscape);


    //this = new MDialog();
    this->setCentralWidget(centralWidget);
}

void ToolPicker::toolSelected(int tool)
{
    emit selectedTool(tool);
   // qDebug() << "Selected " << tool;
    this->disappear();
}
