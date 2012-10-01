#include <MLayout>
#include <MGridLayoutPolicy>
#include <MLabel>
#include <MLinearLayoutPolicy>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <MAbstractCellCreator>
#include <MBasicListItem>
#include <QStringList>
#include <MList>
#include <QDebug>

#include "linewidthdialog.h"
#include "linewidthmodel.h"

#define WIDTH 90
#define HEIGHT 80
#define WIDTH_LANDSCAPE 74
#define HEIGHT_LANDSCAPE 56

class MContentItemCreator : public MAbstractCellCreator<MBasicListItem>
{
public:
    void updateCell(const QModelIndex& index, MWidget * cell) const
    {
        MBasicListItem * contentItem = qobject_cast<MBasicListItem *>(cell);
        contentItem->setStyleName("CommonBasicListItem");
        contentItem->setItemStyle(MBasicListItem::SingleTitle);
        QVariant data = index.data(Qt::DisplayRole);
        QStringList rowData = data.value<QStringList>();
        contentItem->setTitle(rowData[0]);
       // contentItem->setSubtitle(rowData[1]);
    }
};

LineWidthDialog::LineWidthDialog(int lineWidth, QGraphicsWidget *parent)
{
    Q_UNUSED(parent);

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

    // Select item
    QItemSelectionModel *selectionModel = new QItemSelectionModel(model);
    list->setSelectionModel(selectionModel);
    QModelIndex index = model->index(lineWidth);
    selectionModel->select(index, QItemSelectionModel::Select);

    landscapePolicy->addItem(list);
    portraitPolicy->addItem(list);

    this->setCentralWidget(centralWidget);

    connect(list, SIGNAL(itemClicked(QModelIndex)), this, SLOT(itemClicked(QModelIndex)));
}

void LineWidthDialog::itemClicked(const QModelIndex &index)
{
    if (index.row() == 0) {
        emit this->lineWidthChanged(0);
        this->dismiss();
    }
    else if (index.row() == 1) {
        emit this->lineWidthChanged(1);
        this->dismiss();
    }
    else if (index.row() == 2) {
        emit this->lineWidthChanged(2);
        this->dismiss();
    }
    else if (index.row() == 3) {
        emit this->lineWidthChanged(3);
        this->dismiss();
    }
}
