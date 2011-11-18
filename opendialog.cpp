#include <MLayout>
#include <MLinearLayoutPolicy>
#include <QDebug>
#include <MWidgetController>
#include <MAbstractCellCreator>
#include <MWidgetController>
#include <MBasicSheetHeader>
#include <MLabel>
#include <MApplication>
#include <MOrientationChangeEvent>

#include "opendialog.h"
#include "mainpage.h"

class ContentItemCreator : public MAbstractCellCreator<MWidgetController>
{
public:
    ContentItemCreator(OpenDialog* gridPage)
        : MAbstractCellCreator<MWidgetController>(),
          m_gridPage(0),
          m_preferredSize()
    {
        m_gridPage = gridPage;
    }

    virtual MWidget *createCell(const QModelIndex &index, MWidgetRecycler &recycler) const
    {
        MWidget *cell = NULL;

        MediaType m = index.data(Qt::DisplayRole).value<MediaType>();

        if (m.type == MediaType::Image) {
            cell = MListCellCreatorHelper<OpenDialogImageWidget>::createCell(recycler, "", "");
            updateCell(index, cell);
        }
        return cell;
    }

    void updateCell(const QModelIndex &index, MWidget *cell) const;
    QSizeF cellSize() const;

private:
    OpenDialog* m_gridPage;
    QSizeF m_preferredSize;
};

void ContentItemCreator::updateCell(const QModelIndex &index, MWidget *cell) const
{
    if (!cell)
        return;

    MediaType m = index.data(Qt::DisplayRole).value<MediaType>();

    if (m.type == MediaType::Image) {
        OpenDialogImageWidget *imageWidget = qobject_cast<OpenDialogImageWidget*>(cell);

        imageWidget->setStyleName("gridItem");
        imageWidget->setImage(m.image);
        imageWidget->setId(m.path);
        return;
    }

}

QSizeF ContentItemCreator::cellSize() const
{
    return m_gridPage->itemSize();
}

OpenDialog::OpenDialog(QGraphicsWidget *parent)
{
    MWidget *centralWidget = new MWidget;
    MLayout *layout = new MLayout();
    layout->setContentsMargins(0,0,0,0);

    MLinearLayoutPolicy* landscapePolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    MLinearLayoutPolicy* portraitPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);

  //  m_itemSize.setWidth(100);
  //  m_itemSize.setHeight(100);

    m_columnsPortrait = 4;
    m_columnsLandscape = 5;
    m_columnsLandscapeSlider = 0;
    m_columnsPortraitSlider = 0;


    list = new MList(this);
    list->setObjectName("list");
    landscapePolicy->addItem(list);
    portraitPolicy->addItem(list);

    layout->setLandscapePolicy(landscapePolicy);
    layout->setPortraitPolicy(portraitPolicy);


    int initialWidth = MApplication::activeWindow()->visibleSceneSize().width();

  /*  if(MApplication::activeWindow()->orientation() == M::Portrait) {
        list->setColumns(m_columnsPortrait);
        m_itemSize.setWidth(initialWidth / m_columnsPortrait);
        m_itemSize.setHeight(m_itemSize.width());
    }
    else {
        list->setColumns(m_columnsLandscape);
        m_itemSize.setWidth(initialWidth / m_columnsLandscape);
        m_itemSize.setHeight(m_itemSize.width());
    }*/

    m_itemSize.setWidth(110);
    m_itemSize.setHeight(110);

    ContentItemCreator *cellCreator = new ContentItemCreator(this);
    list->setCellCreator(cellCreator);

    QStringList mediaDirs;
    mediaDirs << APP_CATALOG;

    model = new OpenDialogModel(this, m_itemSize.toSize(), mediaDirs);
    model->setObjectName("gridModel");
    list->setItemModel(model);

    connect(list, SIGNAL(itemClicked(QModelIndex)), this, SLOT(itemClicked(QModelIndex)));
    connect(list, SIGNAL(panningStarted()), this, SLOT(pauseLoaders()));
    connect(list, SIGNAL(panningStopped()), this, SLOT(resumeLoaders()));

    centralWidget->setLayout(layout);
    this->setTitle("Choose an image");
    this->setModal(true);
    this->setCentralWidget(centralWidget);
}

void OpenDialog::orientationChangeEvent(MOrientationChangeEvent *event)
{
    MDialog::orientationChangeEvent(event);

    if(event->orientation() == M::Portrait)
        list->setColumns(m_columnsPortrait);

    else
        list->setColumns(m_columnsLandscape);

    this->update();
    this->updateGeometry();
    this->centralWidget()->update();
}

void OpenDialog::itemClicked(const QModelIndex &index)
{
    MediaType::Type type = (MediaType::Type)index.data(OpenDialogModel::Type).toInt();

    if (type == MediaType::Image) {
        QString imageId = index.data(OpenDialogModel::ImageId).toString();
        emit openImage(imageId);
        this->disappear();
    }
}

void OpenDialog::pauseLoaders()
{
    model->pauseLoaders();
}

void OpenDialog::resumeLoaders()
{
    model->resumeLoaders(list->firstVisibleItem().row());
}

