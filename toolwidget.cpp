#include "toolwidget.h"
#include <QDebug>

ToolWidget::ToolWidget(int tool, QString id)
{
    this->tool = tool;
    this->setImage(id);
    this->setMinimumWidth(96);
    this->setMaximumWidth(96);
}

ToolWidget::ToolWidget(int tool, const QPixmap &pixmap, const QPixmap &pixmap_dimmed, bool toggleable, const QPixmap &pixmap_selected, const QPixmap &pixmap_selected_dimmed)
{
    this->tool = tool;
    this->setPixmap(pixmap);
    this->setAspectRatioMode(Qt::KeepAspectRatio);
    this->setZoomFactor(1);
    this->setMinimumWidth(96);
    this->setMaximumWidth(96);
    this->pixmap = pixmap;
    this->pixmap_dimmed = pixmap_dimmed;
    this->toggleable = toggleable;
    if (toggleable) {
        this->toggled = false;
        this->pixmap_selected = pixmap_selected;
        this->pixmap_selected_dimmed = pixmap_selected_dimmed;
    }
}

QRectF ToolWidget::boundingRect() const
{
    return QRectF(20, 4, 68, 48);
}

void ToolWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED (event);

    if(toggleable && toggled)
        this->setPixmap(pixmap_selected_dimmed);
    else this->setPixmap(pixmap_dimmed);
}

void ToolWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED (event);

    emit clicked(tool);

    if(toggleable) {
        toggled = !toggled;
        if(toggled)
            this->setPixmap(pixmap_selected);
        else
            this->setPixmap(pixmap);
    }
    else
        this->setPixmap(pixmap);
}
