#include "toolwidget.h"
#include <QDebug>

ToolWidget::ToolWidget(int tool, QString id)
{
    this->tool = tool;
    this->setImage(id);
    this->setMinimumWidth(96);
    this->setMaximumWidth(96);
}

ToolWidget::ToolWidget(int tool, const QPixmap &pixmap, const QPixmap &pixmap_dimmed)
{
    this->tool = tool;
    this->setPixmap(pixmap);
    this->setAspectRatioMode(Qt::KeepAspectRatio);
    this->setZoomFactor(1);
    this->setMinimumWidth(96);
    this->setMaximumWidth(96);
    this->pixmap = pixmap;
    this->pixmap_dimmed = pixmap_dimmed;
}

QRectF ToolWidget::boundingRect() const
{
    return QRectF(20, 4, 68, 48);
}

void ToolWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    this->setPixmap(pixmap_dimmed);
}

void ToolWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked(tool);
    this->setPixmap(pixmap);
}
