#include <MLayout>
#include <MGridLayoutPolicy>
#include <MLabel>
#include <MLinearLayoutPolicy>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <MAbstractCellCreator>
#include <MContentItem>
#include <QStringList>
#include <MList>
#include <QDebug>

#include "linewidthdialog.h"
#include "linewidthmodel.h"

#define WIDTH 90
#define HEIGHT 80
#define WIDTH_LANDSCAPE 74
#define HEIGHT_LANDSCAPE 56

class MContentItemCreator : public MAbstractCellCreator<MContentItem>
{
public:
    void updateCell(const QModelIndex& index, MWidget * cell) const
    {
        MContentItem * contentItem = qobject_cast<MContentItem *>(cell);
        QVariant data = index.data(Qt::DisplayRole);
        QStringList rowData = data.value<QStringList>();
        contentItem->setTitle(rowData[0]);
       // contentItem->setSubtitle(rowData[1]);
    }
};

LineWidthDialog::LineWidthDialog(QGraphicsWidget *parent)
{
    MWidget *centralWidget = new MWidget;
    MLayout  *layout = new MLayout();

    MLinearLayoutPolicy* landscapePolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    MLinearLayoutPolicy* portraitPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);

    layout->setLandscapePolicy(landscapePolicy);
    layout->setPortraitPolicy(portraitPolicy);

    this->setTitle("Select line width");
    centralWidget->setLayout(layout);
    this->setModal(true);

    list = new MList(this);
    MContentItemCreator * cellCreator = new MContentItemCreator;
    list->setCellCreator(cellCreator);
    LineWidthModel * model = new LineWidthModel;
    list->setItemModel(model);

    landscapePolicy->addItem(list);
    portraitPolicy->addItem(list);

    this->setCentralWidget(centralWidget);

    connect(list, SIGNAL(itemClicked(QModelIndex)), this, SLOT(itemClicked(QModelIndex)));
}

void LineWidthDialog::itemClicked(const QModelIndex &index)
{
    if (index.row() == 0) {
        emit this->lineWidthChanged(0);
        this->disappear();
    }
    else if (index.row() == 1) {
        emit this->lineWidthChanged(1);
        this->disappear();
    }
    else {
        emit this->lineWidthChanged(2);
        this->disappear();
    }
}
