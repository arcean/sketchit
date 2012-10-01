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

#include "toolpickerdialog.h"
#include "toolpickermodel.h"

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
    }
};

ToolPickerDialog::ToolPickerDialog(int toolItem, QGraphicsWidget *parent)
{
    Q_UNUSED(parent);

    MWidget *centralWidget = new MWidget;
    MLayout  *layout = new MLayout();

    MLinearLayoutPolicy* landscapePolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    MLinearLayoutPolicy* portraitPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);

    layout->setLandscapePolicy(landscapePolicy);
    layout->setPortraitPolicy(portraitPolicy);

    this->setTitle("Choose a drawing tool");
    centralWidget->setLayout(layout);
    this->setModal(true);

    list = new MList(this);
    MContentItemCreator *cellCreator = new MContentItemCreator;
    list->setCellCreator(cellCreator);
    ToolPickerModel *model = new ToolPickerModel;
    list->setItemModel(model);

    // Select item
    QItemSelectionModel *selectionModel = new QItemSelectionModel(model);
    list->setSelectionModel(selectionModel);
    QModelIndex index = model->index(toolItem);
    selectionModel->select(index, QItemSelectionModel::Select);

    landscapePolicy->addItem(list);
    portraitPolicy->addItem(list);

    this->setCentralWidget(centralWidget);

    connect(list, SIGNAL(itemClicked(QModelIndex)), this, SLOT(itemClicked(QModelIndex)));
}

void ToolPickerDialog::itemClicked(const QModelIndex &index)
{
    if (index.row() == 0) {
        emit this->toolTypeChanged(0);
        this->dismiss();
    }
    else if (index.row() == 1) {
        emit this->toolTypeChanged(1);
        this->dismiss();
    }
    else if (index.row() == 2) {
        emit this->toolTypeChanged(2);
        this->dismiss();
    }
    else if (index.row() == 3) {
        emit this->toolTypeChanged(3);
        this->dismiss();
    }
    else if (index.row() == 4) {
        emit this->toolTypeChanged(4);
        this->dismiss();
    }
    else {
        emit this->toolTypeChanged(5);
        this->dismiss();
    }
}
