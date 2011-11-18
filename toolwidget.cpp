#include "toolwidget.h"
#include <QDebug>

ToolWidget::ToolWidget(int tool, QString id)
{
    this->tool = tool;
    this->setImage(id);
}

void ToolWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

}

void ToolWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked(tool);
}
