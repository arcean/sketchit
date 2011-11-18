#include "colorcellwidget.h"

#include <QGraphicsSceneResizeEvent>
#include <QDebug>

ColorCellWidget::ColorCellWidget(QColor color, int width, int height, QGraphicsWidget *parent) :
    MWidget(parent)
{
    this->color = color;
    this->width = width;
    this->height = height;
    this->resize(width, height);
    this->setMinimumWidth(96);
    this->setMaximumWidth(96);
}

ColorCellWidget::~ColorCellWidget()
{

}

void ColorCellWidget::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    QSizeF size = event->newSize();
    this->height = size.height();
}

QRectF ColorCellWidget::boundingRect() const
{
    return QRectF(20, 4, 68, 48);
}

void ColorCellWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(height == 72) {
        painter->fillRect(20, 10, 48, 48, "lightgray");
        painter->fillRect(23, 13, 42, 42, color);
    }
    else {
        painter->fillRect(20, 4, 48, 48, "lightgray");
        painter->fillRect(23, 7, 42, 42, color);
    }
}

void ColorCellWidget::setColor(QColor color)
{
    this->color = color;
}

void ColorCellWidget::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    emitSignalPressed();
}

void ColorCellWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
    emitSignalClicked();
}

void ColorCellWidget::emitSignalPressed()
{
    emit this->signalPressed();
}

void ColorCellWidget::emitSignalClicked()
{
    emit this->signalClicked(this->color);
}
