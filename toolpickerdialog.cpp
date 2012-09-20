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

#include "toolpickerdialog.h"
#include "toolpickermodel.h"

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
    }
};

ToolPickerDialog::ToolPickerDialog(QGraphicsWidget *parent)
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

    landscapePolicy->addItem(list);
    portraitPolicy->addItem(list);

    this->setCentralWidget(centralWidget);

    connect(list, SIGNAL(itemClicked(QModelIndex)), this, SLOT(itemClicked(QModelIndex)));
}

void ToolPickerDialog::itemClicked(const QModelIndex &index)
{
    if (index.row() == 0) {
        emit this->toolTypeChanged(0);
        this->disappear();
    }
    else if (index.row() == 1) {
        emit this->toolTypeChanged(1);
        this->disappear();
    }
    else if (index.row() == 2) {
        emit this->toolTypeChanged(2);
        this->disappear();
    }
    else if (index.row() == 3) {
        emit this->toolTypeChanged(3);
        this->disappear();
    }
    else if (index.row() == 4) {
        emit this->toolTypeChanged(4);
        this->disappear();
    }
    else {
        emit this->toolTypeChanged(5);
        this->disappear();
    }
}
