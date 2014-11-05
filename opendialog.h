#ifndef OPENDIALOG_H
#define OPENDIALOG_H

#include <QWidget>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include <MDialog>
#include <MList>
#include <MAction>
#include <MSlider>
#include <MSheet>

#include "colorcell.h"
#include "opendialogmodel.h"
#include "opendialogimagewidget.h"

class OpenDialog : public MDialog
{
    Q_OBJECT
public:
    explicit OpenDialog(QGraphicsWidget *parent = 0, const QString &currentFileName = "");
    ~OpenDialog();

    QSizeF itemSize() const
    {
        return m_itemSize;
    }

protected:
    virtual void orientationChangeEvent(MOrientationChangeEvent *event);

public Q_SLOTS:
    void itemClicked(const QModelIndex &index);

signals:
    void openImage(QString openImage);

private slots:
    void pauseLoaders();
    void resumeLoaders();

private:
    MList *list;
    OpenDialogModel *model;
    MAction *actionConfiguration;
    QSizeF m_itemSize;

    int m_columnsPortrait;
    int m_columnsLandscape;

    MSlider* m_columnsLandscapeSlider;
    MSlider* m_columnsPortraitSlider;
};

#endif // OPENDIALOG_H
