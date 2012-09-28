#include <QGraphicsSceneResizeEvent>
#include <QDebug>

#include "colorcell.h"

#define NORMAL_STATE 0
#define SELECTED_STATE 6

ColorCell::ColorCell(QColor color, int width, int height, bool isSelect, int id, QGraphicsWidget *parent) :
    QGraphicsWidget(parent),
    color(color),
    width(width),
    height(height),
    isSelect(isSelect),
    id(id)
{
    if (isSelect)
        this->margin = SELECTED_STATE;
    else
        this->margin = NORMAL_STATE;

    this->resize(this->width, this->height);
    this->closeDialog = false;

    showAnimation = new VariantAnimator();
    showAnimation->setStartValue(NORMAL_STATE);
    showAnimation->setEndValue(SELECTED_STATE);
    showAnimation->setDuration(200);
    showAnimation->setEasingCurve(QEasingCurve::InQuad);

    connect(showAnimation, SIGNAL(valueChanged(QVariant)), this, SLOT(expandAnimation(QVariant)));
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
    int widthH = width + (margin * 2);
    int heightH = height + (margin * 2);

    return QRectF(-margin, -margin, widthH, heightH);
}

void ColorCell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    int widthH = width + (margin * 2);
    int heightH = height + (margin * 2);

    painter->fillRect(-margin, -margin, widthH, heightH, color);
}

void ColorCell::expandAnimation(const QVariant &value)
{
    if (isSelect) {
        if (margin != value.toInt()) {
            margin = value.toInt();

            update();
        }

        // Close the picker, after the animation is finished.
        if (closeDialog && margin == SELECTED_STATE)
            emit this->signalClicked(this->color, this->id);
    }
    else {
        if (SELECTED_STATE - margin != value.toInt()) {
            margin = SELECTED_STATE - value.toInt();

            update();
        }
    }
}

void ColorCell::setNormalState()
{
    // Run the following code only for a cell in SELECTED_STATE.
    if (margin == SELECTED_STATE) {
        isSelect = false;
        showAnimation->start();
    }
}

void ColorCell::setSelectedState()
{
    // Run the following code only for a cell in NORMAL_STATE.
    if (margin == NORMAL_STATE) {
        isSelect = true;
        showAnimation->start();
    }
}

void ColorCell::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    isSelect = true;
    emitSignalPressed(false);

    showAnimation->start();
}

void ColorCell::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
    if (isUnderMouse())
        emitSignalClicked();
    else {
        setNormalState();
        emitSignalPressed(true);
    }
}

void ColorCell::emitSignalPressed(bool selectIdToSelect)
{
    emit this->signalPressed(this->id, selectIdToSelect);
}

void ColorCell::emitSignalClicked()
{
    if (showAnimation->state() == QAbstractAnimation::Running)
        closeDialog = true;
    else
        emit this->signalClicked(this->color, this->id);
}
