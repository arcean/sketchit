#include "colorcellwidget.h"

#include <QGraphicsSceneResizeEvent>
#include <QDebug>

#define NORMAL_STATE 0
#define SELECTED_STATE 6

ColorCellWidget::ColorCellWidget(QColor color, int width, int height, QGraphicsWidget *parent) :
    MWidget(parent)
{
    this->color = color;
    this->width = width;
    this->height = height;
    this->resize(width, height);
    this->setMinimumWidth(96);
    this->setMaximumWidth(96);

    this->closeDialog = false;

    pressAnimation = new VariantAnimator();
    pressAnimation->setStartValue(NORMAL_STATE);
    pressAnimation->setEndValue(SELECTED_STATE);
    pressAnimation->setDuration(200);
    pressAnimation->setEasingCurve(QEasingCurve::InQuad);

    connect(pressAnimation, SIGNAL(valueChanged(QVariant)), this, SLOT(expandAnimation(QVariant)));
}

ColorCellWidget::~ColorCellWidget()
{
    delete pressAnimation;
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
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if(height == 72) {
       // painter->fillRect(20, 10, 48, 48, "lightgray");
       // painter->fillRect(23, 13, 42, 42, color);
        int x = 20 - margin;
        int y = 10 - margin;
        int w = 48 + (margin * 2);
        int h = 48 + (margin * 2);

        painter->fillRect(x, y, w, h, color);
    }
    else {
       // painter->fillRect(20, 4, 48, 48, "lightgray");
       // painter->fillRect(23, 7, 42, 42, color);
        int x = 20 - margin;
        int y = 4 - margin;
        int w = 48 + (margin * 2);
        int h = 48 + (margin * 2);

        painter->fillRect(x, y, w, h, color);
    }
}

void ColorCellWidget::setColor(QColor color)
{
    this->color = color;
}

void ColorCellWidget::expandAnimation(const QVariant &value)
{
    if (isSelect) {
        if (margin != value.toInt()) {
            margin = value.toInt();

            update();
        }

        // Close the picker, after the animation is finished.
        if (closeDialog && margin == SELECTED_STATE) {
            emit this->signalClicked(this->color);
            closeDialog = false;
            setNormalState();
        }
    }
    else {
        if (SELECTED_STATE - margin != value.toInt()) {
            margin = SELECTED_STATE - value.toInt();

            update();
        }
    }
}

void ColorCellWidget::setNormalState()
{
    // Run the following code only for a cell in SELECTED_STATE.
    if (margin == SELECTED_STATE) {
        isSelect = false;
        pressAnimation->start();
    }
}

void ColorCellWidget::setSelectedState()
{
    // Run the following code only for a cell in NORMAL_STATE.
    if (margin == NORMAL_STATE) {
        isSelect = true;
        pressAnimation->start();
    }
}

void ColorCellWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    isSelect = true;
    emitSignalPressed();

    pressAnimation->start();
    event->accept();
}

void ColorCellWidget::releaseFunc()
{
    if (isUnderMouse()) {
        emitSignalClicked();
        setNormalState();
    }
    else {
        setNormalState();
        emitSignalPressed();
        closeDialog = false;
    }
}

void ColorCellWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    releaseFunc();
    event->accept();
}

void ColorCellWidget::emitSignalPressed()
{
    emit this->signalPressed();
}

void ColorCellWidget::emitSignalClicked()
{
    if (pressAnimation->state() == QAbstractAnimation::Running)
        closeDialog = true;
    else
        emit this->signalClicked(this->color);
}
