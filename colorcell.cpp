#include "colorcell.h"

#include <QGraphicsSceneResizeEvent>

ColorCell::ColorCell(QColor color, int width, int height, QGraphicsWidget *parent) :
    QGraphicsWidget(parent)
{
    this->color = color;
    this->width = width;
    this->height = height;
    this->resize(width, height);
}

ColorCell::~ColorCell()
{

}

void ColorCell::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    QSizeF size = event->newSize();
    this->width = size.width();
    this->height = size.height();
}

QRectF ColorCell::boundingRect() const
{
    return QRectF(0, 0, width, height);
}

void ColorCell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->fillRect(0, 0, width, height, color);
}

void ColorCell::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    emitSignalPressed();
}

void ColorCell::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
    emitSignalClicked();
}

void ColorCell::emitSignalPressed()
{
    emit this->signalPressed();
}

void ColorCell::emitSignalClicked()
{
    emit this->signalClicked(this->color);
}
