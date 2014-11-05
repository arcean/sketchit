#include <QGraphicsSceneResizeEvent>
#include <QTime>
#include <QDebug>
#include <MApplication>
#include <MWindow>

#include "colorcell.h"

#define NORMAL_STATE 0
#define SELECTED_STATE 6

#define SHAKE_DURATION 220
#define SHAKE_MAX_MOVE 6

//#define ENABLE_SHAKE

ColorCell::ColorCell(QColor color, int width, int height, bool isSelect, int id, bool shake, QGraphicsWidget *parent) :
    QGraphicsWidget(parent),
    color(color),
    width(width),
    height(height),
    isSelect(isSelect),
    id(id)
{
#ifndef ENABLE_SHAKE
    Q_UNUSED(shake)
#endif
    if (isSelect)
        this->margin = SELECTED_STATE;
    else
        this->margin = NORMAL_STATE;

    this->resize(this->width, this->height);
    this->closeDialog = false;
    this->wasPressed = false;

    showAnimation = new VariantAnimator();
    showAnimation->setStartValue(NORMAL_STATE);
    showAnimation->setEndValue(SELECTED_STATE);
    showAnimation->setDuration(200);
    showAnimation->setEasingCurve(QEasingCurve::InQuad);

    connect(showAnimation, SIGNAL(valueChanged(QVariant)), this, SLOT(expandAnimation(QVariant)));

#ifdef ENABLE_SHAKE
    dX = dY = 0;

    if (shake) {
        shakeAnimation = new VariantAnimator();
        shakeAnimation->setStartValue(NORMAL_STATE);
        shakeAnimation->setEndValue(SHAKE_MAX_MOVE);
        shakeAnimation->setDuration(SHAKE_DURATION);
        shakeAnimation->setEasingCurve(QEasingCurve::InQuad);

        window = MApplication::activeWindow();

        connect(shakeAnimation, SIGNAL(valueChanged(QVariant)), this, SLOT(shakeAnimationFunc(QVariant)));
        connect(shakeAnimation, SIGNAL(finished()), this, SLOT(launchShake()));
       /* connect(window, SIGNAL(displayExited()), this, SLOT(handleVisibilityOff()));
        connect(window, SIGNAL(displayEntered()), this, SLOT(handleVisibilityOn()));
        connect(window, SIGNAL(switcherEntered()), this, SLOT(handleVisibilityOff()));
        connect(window, SIGNAL(switcherExited()), this, SLOT(handleVisibilityOn()));*/

       // shakeTimer->start(SHAKE_DURATION);
        launchShake();
    }
#endif
}

ColorCell::~ColorCell()
{
#ifdef ENABLE_SHAKE
    handleVisibilityOff();
    delete shakeAnimation;
#endif

    delete showAnimation;
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

#ifdef ENABLE_SHAKE
    return QRectF(-margin + dX, -margin + dY, widthH, heightH);
#else
    return QRectF(-margin, -margin, widthH, heightH);
#endif
}

void ColorCell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    int widthH = width + (margin * 2);
    int heightH = height + (margin * 2);

#ifdef ENABLE_SHAKE
    painter->fillRect(-margin + dX, -margin + dY, widthH, heightH, color);
#else
    painter->fillRect(-margin, -margin, widthH, heightH, color);
#endif
}

#ifdef ENABLE_SHAKE
void ColorCell::handleVisibilityOn()
{
    if (!window->isInSwitcher())
        launchShake();
}

void ColorCell::handleVisibilityOff()
{
    shakeAnimation->stop();
    dX = dY = 0;

    update();
}

void ColorCell::shakeAnimationFunc(const QVariant &value)
{
    qDebug() << "BALALALA" << value.toInt() << "DX" << dX << "DY" << dY;
    if (dX < 0)
        dX = (value.toInt() * -1);
    else if (dX > 0)
        dX = value.toInt();

    if (dY < 0)
        dY = (value.toInt() * -1);
    else if (dY > 0)
        dY = value.toInt();

    update();
}

int ColorCell::randInt(int low, int high)
{
    // Random number between low and high
    return qrand() % ((high + 1) - low) + low;
}
#endif

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

void ColorCell::setNormalState(bool forced)
{
    // Run the following code only for a cell in SELECTED_STATE.
    if ((margin == SELECTED_STATE && wasPressed) || (margin == SELECTED_STATE && forced)) {
        isSelect = false;
        showAnimation->start();
    }
}

void ColorCell::setSelectedState(bool wasPressed)
{
    // Run the following code only for a cell in NORMAL_STATE.
    if (margin == NORMAL_STATE) {
        isSelect = true;
        this->wasPressed = wasPressed;
        showAnimation->start();
    }
}

#ifdef ENABLE_SHAKE
void ColorCell::launchShake()
{
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    int random = randInt(0,2);
    if (random == 0)
        dX = 0;
    else if (random == 1)
        dX = 1;
    else if (random == 2)
        dX = -1;

    random = randInt(0,2);
    if (random == 0)
        dY = 0;
    else if (random == 1)
        dY = 1;
    else if (random == 2)
        dY = -1;

    shakeAnimation->start();
}
#endif

void ColorCell::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    isSelect = true;
    emitSignalPressed(false);

    setSelectedState(true);
}

void ColorCell::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
    if (isUnderMouse())
        emitSignalClicked();
    else {
        setNormalState(false);
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
