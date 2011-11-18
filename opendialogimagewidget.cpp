#include <QGraphicsSceneMouseEvent>
#include <QPropertyAnimation>

#include "opendialogimagewidget.h"

OpenDialogImageWidget::OpenDialogImageWidget(QGraphicsItem *parent)
    : MImageWidget(parent),
    m_id(),
    opacityAnimation(0)
{
    opacityAnimation = new QPropertyAnimation(this, "opacity", this);
    opacityAnimation->setStartValue(0.0);
    opacityAnimation->setEndValue(1.0);
    opacityAnimation->setDuration(300);
    this->setMinimumSize(100, 100);
    this->setMaximumSize(100, 100);
}

void OpenDialogImageWidget::setId(const QString& id)
{
    showAnimated();
    m_id = id;
}

QString OpenDialogImageWidget::id()
{
    return m_id;
}

void OpenDialogImageWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    MImageWidget::mousePressEvent(event);
    event->accept();
}

void OpenDialogImageWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    MImageWidget::mouseReleaseEvent(event);
    emit clicked();
}

void OpenDialogImageWidget::showAnimated()
{
    opacityAnimation->start();
}
