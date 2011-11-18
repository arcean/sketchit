#include <QDebug>
#include <QGraphicsSceneResizeEvent>
#include <QPen>
#include <QTransform>
#include <QSettings>
#include <MApplication>

#include "paintingarea.h"
#include "mainpage.h"

PaintingArea::PaintingArea(int width, int height, MWidget *parent) :
    MWidget(parent)
{
    setAttribute(Qt::WA_AcceptTouchEvents);
    setAttribute(Qt::WA_StaticContents);   
    this->setAcceptTouchEvents(true);
    this->lineWidthSelected = 6;

    color = QColor("black");

    createNewImage();
    ok = true;
    panningMode = false;
    setIsImageModified(false);
    this->parent = parent;
    scaleFactor = 1.0;
}

void PaintingArea::openImage (QString newImage)
{
    image->load(newImage, "PNG");
    standardZoom();
    setIsImageModified(false);
    this->width = image->width();
    this->height = image->height();
    this->resize(image->width(), image->height());
    this->setGeometry(0, 0, image->width(), image->height());
    this->setMinimumSize(image->width(), image->height());
    this->setMaximumSize(image->width(), image->height());
}

void PaintingArea::saveImage (QString filename)
{
    if(filename.count(".png") < 1) {
        filename += ".png";
    }

    QFile file(filename);
    if(file.exists())
        bool r1 = file.remove();

    bool ret = image->save(filename, "PNG");

    setIsImageModified(false);
}

QRectF PaintingArea::boundingRect() const
{
    return QRectF(0, 0, width, height);
}

void PaintingArea::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRect rect = QRect(0, 0, this->width, this->height);

    painter->drawImage(rect.topLeft(), *image);
}

void PaintingArea::drawLineFromTo(QPointF from, QPointF to)
{
    QPainter painter(image);
    QPen pen;

    pen.setWidth(lineWidthSelected);

    pen.setBrush(QBrush(color));
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);

    painter.setPen(pen);
    painter.setBrush(QBrush(color));
    painter.drawLine(from, to);
    painter.end();
}

void PaintingArea::drawFinal(QPointF from, QPointF to)
{
    QPainter painter(image);
    QPen pen;

    if(toolTypeSelected == line)
        pen.setWidth(lineWidthSelected);
    else
        pen.setWidth(0);

    pen.setBrush(QBrush(color));
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);

    painter.setPen(pen);
    painter.setBrush(QBrush(color));
/*
    updateLowPoint(from);
    updateLowPoint(to);
    updateHighPoint(from);
    updateHighPoint(to);*/

    if(from.x() < lastPoint.x()) {
        if(from.y() < lastPoint.y()) {
            lowPoint.setX(from.x()-5);
            lowPoint.setY(from.y()-5);
            highPoint.setX(lastPoint.x()+5);
            highPoint.setY(lastPoint.y()+5);
        }
        else {
            lowPoint.setX(from.x()-5);
            lowPoint.setY(lastPoint.y()-5);
            highPoint.setX(lastPoint.x()+5);
            highPoint.setY(from.y()+5);
        }
    }
    else {
        if(from.y() < lastPoint.y()) {
            lowPoint.setX(lastPoint.x()-5);
            lowPoint.setY(from.y()-5);
            highPoint.setX(from.x()+5);
            highPoint.setY(lastPoint.y()+5);
        }
        else {
            lowPoint.setX(lastPoint.x()-5);
            lowPoint.setY(lastPoint.y()-5);
            highPoint.setX(from.x()+5);
            highPoint.setY(from.y()+5);
        }
    }


    QRectF target(lowPoint, highPoint);

    int targetWidth = target.width();
    int targetHeight = target.height();

    switch (this->toolTypeSelected) {
    case rectangle: {

        break;
    }
    case elipse: {
        target.setTopLeft(QPointF(lowPoint.x() - (targetWidth / 8), lowPoint.y() - (targetWidth / 8)));
        target.setBottomRight(QPointF(highPoint.x() + (targetWidth / 10), highPoint.y() + (targetWidth / 10)));
        break;
    }
    case line: {
        target.setTopLeft(QPointF(lowPoint.x() - (targetWidth ), lowPoint.y() - (targetWidth )));
        target.setBottomRight(QPointF(highPoint.x() + (targetWidth ), highPoint.y() + (targetWidth)));
        break;
    }
    }


    if (from.x() < to.x() && from.y() < to.y())
    {
        if(target.width() > 1 && target.height() > 1)
            painter.drawImage(target, *backup_image, target);

        switch (this->toolTypeSelected) {
        case line: {
            painter.drawLine(from.x(), from.y(), to.x(), to.y() );
            break;
        }
        case rectangle: {
            painter.drawRect(from.x(), from.y(), to.x() - from.x() - 1, to.y() - from.y() - 1);
            break;
        }
        case elipse: {
            painter.drawEllipse(from.x(), from.y(), to.x() - from.x() - 1, to.y() - from.y() - 1);
            break;
        }
        }
    }
    else if (from.x() < to.x() && from.y() > to.y())
    {
        if(target.width() > 1 && target.height() > 1)
            painter.drawImage(target, *backup_image, target);

        switch (this->toolTypeSelected) {
        case line: {
            painter.drawLine(from.x(), from.y(), to.x(), to.y() );
            break;
        }
        case rectangle: {
            painter.drawRect(from.x(), to.y(), to.x() - from.x() - 1, from.y() - to.y() - 1);
            break;
        }
        case elipse: {
            painter.drawEllipse(from.x(), to.y(), to.x() - from.x() - 1, from.y() - to.y() - 1);
            break;
        }
        }
    }
    else if (from.x() > to.x() && from.y() < to.y())
    {
        if(target.width() > 1 && target.height() > 1)
            painter.drawImage(target, *backup_image, target);

        switch (this->toolTypeSelected) {
        case line: {
            painter.drawLine(from.x(), from.y(), to.x(), to.y() );
            break;
        }
        case rectangle: {
            painter.drawRect(to.x(), from.y(), from.x() - to.x() - 1, to.y() - from.y() - 1);
            break;
        }
        case elipse: {
            painter.drawEllipse(to.x(), from.y(), from.x() - to.x() - 1, to.y() - from.y() - 1);
            break;
        }
        }
    }
    else if (from.x() > to.x() && from.y() > to.y())
    {
        if(target.width() > 1 && target.height() > 1)
            painter.drawImage(target, *backup_image, target);

        switch (this->toolTypeSelected) {
        case line: {
            painter.drawLine(from.x(), from.y(), to.x(), to.y() );
            break;
        }
        case rectangle: {
            painter.drawRect(to.x(), to.y(), from.x() - to.x() - 1, from.y() - to.y() - 1);
            break;
        }
        case elipse: {
            painter.drawEllipse(to.x(), to.y(), from.x() - to.x() - 1, from.y() - to.y() - 1);
            break;
        }
        }
    }
    painter.end();
    update(target);
}

void PaintingArea::updateLowPoint(QPointF point)
{
    if(lowPoint.x() > point.x()) {
        lowPoint.setX(point.x());
    }
    if(lowPoint.y() > point.y()) {
        lowPoint.setY(point.y());
    }
}

void PaintingArea::updateHighPoint(QPointF point)
{
    if(highPoint.x() < point.x()) {
        highPoint.setX(point.x());
    }
    if(highPoint.y() < point.y()) {
        highPoint.setY(point.y());
    }
}

void PaintingArea::setActualEreasingAreaRectangle(QPointF from, QPointF to, int direction)
{

    qDebug() << "FROM: " << from.x() << ":" << from.y() << " TO: " << to.x() << ":" << to.y() << " LastPoint: " << lastPoint.x() << ":" << lastPoint.y();

    /* direction == 1 */
    if(direction == 1) {
        // from.x() < to.x() && from.y() < to.y()

        /* If covers out last region, do not erease */
        if(to.x() > lastPoint.x() && to.y() > lastPoint.y()) {
            previousDirection = 1;
            return;
        }
        else {
            QPainter painter(image);
            /* Check previous direction */
            if(previousDirection == 2) {
                lowPoint.setX(from.x());
                lowPoint.setY(lastPoint.y());
                highPoint.setX(to.x());
                highPoint.setY(from.y());
                QRectF ereaseTargetDirection1(lowPoint, highPoint);

                painter.drawImage(ereaseTargetDirection1, *backup_image, ereaseTargetDirection1);
                qDebug() << "CRI DONE";
            }
            else if(previousDirection == 4) {
                lowPoint.setX(lastPoint.x());
                lowPoint.setY(from.y());
                highPoint.setX(from.x());
                highPoint.setY(lastPoint.y());
                QRectF ereaseTargetDirection1(lowPoint, highPoint);

                painter.drawImage(ereaseTargetDirection1, *backup_image, ereaseTargetDirection1);
            }

            /* Erease the right part of the region */
            lowPoint.setX(to.x());
            lowPoint.setY(from.y());
            highPoint.setX(lastPoint.x()+1);
            highPoint.setY(lastPoint.y()+1);
            QRectF ereaseTarget(lowPoint, highPoint);

            if(ereaseTarget.width() >= 1 && ereaseTarget.height() >= 1)
                painter.drawImage(ereaseTarget, *backup_image, ereaseTarget);

            /* Erease the bottom part of the region */
            lowPoint.setX(from.x());
            lowPoint.setY(to.y());
            highPoint.setX(to.x());
            highPoint.setY(lastPoint.y()+1);
            QRectF ereaseTarget2(lowPoint, highPoint);
            ereaseTarget2.setTopLeft(lowPoint);
            ereaseTarget2.setBottomRight(highPoint);

            if(ereaseTarget2.width() >= 1 && ereaseTarget2.height() >= 1)
                painter.drawImage(ereaseTarget2, *backup_image, ereaseTarget2);
            painter.end();
            previousDirection = 1;
            return;
        }
    }
    /* direction == 2 */
    else if(direction == 2) {
        // from.x() < to.x() && from.y() > to.y()

        /* If covers out last region, do not erease */
        if(to.x() > lastPoint.x() && to.y() < lastPoint.y()) {
            qDebug() << "COVERING";
            previousDirection = 2;
            return;
        }
        else {
            QPainter painter(image);
            /* Check previous direction */
            if(previousDirection == 1) {
                lowPoint.setX(from.x());
                lowPoint.setY(from.y());
                highPoint.setX(to.x());
                highPoint.setY(lastPoint.y());
                QRectF ereaseTargetDirection1(lowPoint, highPoint);

                painter.drawImage(ereaseTargetDirection1, *backup_image, ereaseTargetDirection1);
                qDebug() << "CRI DONE";
            }

            /* Erease the right part of the region */
            lowPoint.setX(to.x());
            lowPoint.setY(lastPoint.y());
            highPoint.setX(lastPoint.x()+1);
            highPoint.setY(from.y());
            QRectF ereaseTarget(lowPoint, highPoint);

            if(ereaseTarget.width() >= 1 && ereaseTarget.height() >= 1)
                painter.drawImage(ereaseTarget, *backup_image, ereaseTarget);

            /* Erease the upper part of the region */
            lowPoint.setX(from.x()-1);
            lowPoint.setY(lastPoint.y());
            highPoint.setX(to.x());
            highPoint.setY(to.y());
            QRectF ereaseTarget2(lowPoint, highPoint);
            ereaseTarget2.setTopLeft(lowPoint);
            ereaseTarget2.setBottomRight(highPoint);

            if(ereaseTarget2.width() >= 1 && ereaseTarget2.height() >= 1)
                painter.drawImage(ereaseTarget2, *backup_image, ereaseTarget2);
            painter.end();
            previousDirection = 2;
            return;
        }
    }
    /* direction == 3 */
    else if(direction == 3) {
        // from.x() > to.x() && from.y() < to.y()

        /* If covers out last region, do not erease */
        if(to.x() < lastPoint.x() && to.y() > lastPoint.y()) {
            previousDirection = 3;
            qDebug() << "COVERING";
            return;
        }
        else {
            QPainter painter(image);
            /* Erease the left part of the region */
            lowPoint.setX(lastPoint.x());
            lowPoint.setY(from.y());
            highPoint.setX(to.x());
            highPoint.setY(lastPoint.y());
            QRectF ereaseTarget(lowPoint, highPoint);

            if(ereaseTarget.width() >= 1 && ereaseTarget.height() >= 1)
                painter.drawImage(ereaseTarget, *backup_image, ereaseTarget);

            /* Erease the bottom part of the region */
            lowPoint.setX(to.x());
            lowPoint.setY(to.y());
            highPoint.setX(from.x());
            highPoint.setY(lastPoint.y());
            QRectF ereaseTarget2(lowPoint, highPoint);
            ereaseTarget2.setTopLeft(lowPoint);
            ereaseTarget2.setBottomRight(highPoint);

            if(ereaseTarget2.width() >= 1 && ereaseTarget2.height() >= 1)
                painter.drawImage(ereaseTarget2, *backup_image, ereaseTarget2);
            painter.end();
            previousDirection = 3;
            return;
        }
    }
    /* direction == 4 */
    else if(direction == 4) {
        // from.x() > to.x() && from.y() > to.y()

        /* If covers out last region, do not erease */
        if(to.x() < lastPoint.x() && to.y() < lastPoint.y()) {
            previousDirection = 4;
            qDebug() << "COVERING";
            return;
        }
        else {
            QPainter painter(image);
            /* Erease the left part of the region */
            lowPoint.setX(lastPoint.x());
            lowPoint.setY(lastPoint.y());
            highPoint.setX(to.x());
            highPoint.setY(from.y());
            QRectF ereaseTarget(lowPoint, highPoint);

            if(ereaseTarget.width() > 1 && ereaseTarget.height() > 1)
                painter.drawImage(ereaseTarget, *backup_image, ereaseTarget);

            /* Erease the upper part of the region */
            lowPoint.setX(to.x());
            lowPoint.setY(lastPoint.y());
            highPoint.setX(from.x());
            highPoint.setY(to.y());
            QRectF ereaseTarget2(lowPoint, highPoint);
            ereaseTarget2.setTopLeft(lowPoint);
            ereaseTarget2.setBottomRight(highPoint);

            if(ereaseTarget2.width() >= 1 && ereaseTarget2.height() >= 1)
                painter.drawImage(ereaseTarget2, *backup_image, ereaseTarget2);
            painter.end();
            previousDirection = 4;
            return;
        }
    }
}

bool PaintingArea::isDamaged(QPointF from, QPointF to, int direction)
{
    if (direction == 1) {
        if(from.x() > to.x() || from.y() > to.y()) {
            if(from.x() > to.x())
                damageCount = 1;
            if(from.y() > to.y())
                damageCount = 2;
            if(from.x() > to.x() && from.y() > to.y())
                damageCount = 3;
            qDebug() << "DAMAGE 1";
            return true;
        }
    }
    else if (direction == 2) {
        if((from.x() > to.x()) || (from.y() < to.y())) {
            qDebug() << "DAMAGE 2";
            return true;
        }
    }
    else if (direction == 3) {
        if(from.x() < to.x() || from.y() > to.y()) {
            qDebug() << "DAMAGE 3";
            return true;
        }
    }
    else if (direction == 4) {
        if(from.x() < to.x() || from.y() < to.y()) {
            qDebug() << "DAMAGE 4";
            return true;
        }
    }
qDebug() << "NO DAMAGE";
    return false;
}

void PaintingArea::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(panningMode) {
        standardZoom();
    }
}

void PaintingArea::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(!panningMode) {
        switch (this->toolTypeSelected) {
            case line:
            case elipse:
            case rectangle: {
            touchBegin = event->pos();
            lowPoint = event->pos();
            highPoint = event->pos();
            if(backup_image)
                 backup_image->~QImage();
             backup_image = new QImage(*image);
            }
            default: {

            }
        }
    }
}

void PaintingArea::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(!panningMode) {
        switch (this->toolTypeSelected) {
            case line:
            case elipse:
            case rectangle: {
                drawFinal(touchBegin, event->pos());
                setIsImageModified(true);
            }
            default: {

            }
        }
    }
}

bool PaintingArea::event(QEvent *event)
{
    if(!panningMode) {
        switch (event->type()) {
        case QEvent::TouchBegin:
        {
            switch (this->toolTypeSelected) {
                case line:
                case elipse:
                case rectangle: {
                    QList<QTouchEvent::TouchPoint> touchPoints = static_cast<QTouchEvent *>(event)->touchPoints();
                    if(touchPoints.length() > 0)
                    {
                       currPoint = lastPoint = touchBegin = touchPoints[0].lastPos();
                       lowPoint = touchPoints[0].lastPos();
                       highPoint = touchPoints[0].lastPos();
                       damageCount = 0;
                       if(backup_image)
                            backup_image->~QImage();
                        backup_image = new QImage(*image);
                        setIsImageModified(true);
                    }
                }
                default: {

                }
            }
        }
            case QEvent::TouchUpdate:
            {
                switch (this->toolTypeSelected) {
                    case line:
                    case elipse:
                    case rectangle: {
                        QList<QTouchEvent::TouchPoint> touchPoints = static_cast<QTouchEvent *>(event)->touchPoints();
                        if(touchPoints.length() > 0)
                        {
                            lastPoint = currPoint;
                            currPoint = touchPoints[0].pos();
                            drawFinal(touchBegin, touchPoints[0].pos());
                        }
                        setIsImageModified(true);
                    }
                    default: {

                    }
                }
            }
            case QEvent::TouchEnd:
            {
                setIsImageModified(true);
                QList<QTouchEvent::TouchPoint> touchPoints = static_cast<QTouchEvent *>(event)->touchPoints();
                foreach (const QTouchEvent::TouchPoint &touchPoint, touchPoints) {
                    switch (touchPoint.state()) {
                    case Qt::TouchPointStationary:
                        continue;
                    default:
                        {
                            switch (this->toolTypeSelected) {
                                case rubber:
                                case finger: {
                                    QRectF rect = touchPoint.rect();
                                    if (rect.isEmpty()) {
                                        qreal diameter = qreal(50) * touchPoint.pressure();
                                        rect.setSize(QSizeF(diameter, diameter));
                                    }

                                    QPainter painter(image);
                                    painter.setPen(Qt::NoPen);
                                    if(rubberMode)
                                        painter.setBrush(QBrush("white"));
                                    else
                                        painter.setBrush(QBrush(color));
                                    painter.drawEllipse(rect);
                                    painter.end();

                                    int rad = 2;
                                    update(rect.toRect().adjusted(-rad,-rad, +rad, +rad));
                                    break;
                                }
                                case pencil: {
                                    drawLineFromTo(touchPoint.lastPos(), touchPoint.pos());
                                    update(touchPoint.rect().x(), touchPoint.rect().y(), touchPoint.rect().width(), touchPoint.rect().height());
                                    break;
                                }
                                case line:
                                case elipse:
                                case rectangle: {

                                    break;
                                }
                                default:
                                    break;
                            }
                        }
                    }
                }
                break;
            }
            default:
                return QGraphicsWidget::event(event);
        }

    }
    else {
            switch (event->type()) {
            case QEvent::TouchBegin:
            case QEvent::TouchUpdate:
            case QEvent::TouchEnd:
            {
                QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
                QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
                if (touchPoints.count() == 2) {
                    // determine scale factor
                    const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
                    const QTouchEvent::TouchPoint &touchPoint1 = touchPoints.last();
                    qreal currentScaleFactor =
                            QLineF(touchPoint0.pos(), touchPoint1.pos()).length()
                            / QLineF(touchPoint0.startPos(), touchPoint1.startPos()).length();

                    bool quit = false;

                    int windowWidth = MApplication::activeWindow()->visibleSceneSize().width();
                    int windowHeight = MApplication::activeWindow()->visibleSceneSize().height();
                    int width = image->size().width();
                    int height = image->size().height();

                    if (touchEvent->touchPointStates() & Qt::TouchPointReleased) {
                        // if one of the fingers is released, remember the current scale
                        // factor so that adding another finger later will continue zooming
                        // by adding new scale factor to the existing remembered value.
                        double tempFactor = scaleFactor * currentScaleFactor;
                        if (( (tempFactor * width) > (windowWidth * 3)) &&
                              ((tempFactor * height) > (windowHeight * 3))){
                            quit = false;
                            scaleFactor = lastZoomIn - 0.02;
                        }
                        else if (( (tempFactor * width) < (windowWidth - 120)) &&
                                 ((tempFactor * height) < (windowHeight - 120)) ) {
                            quit = false;
                            scaleFactor = lastZoomOut + 0.02;
                        }
                        else {
                            scaleFactor *= currentScaleFactor;
                            quit = false;
                        }
                        currentScaleFactor = 1;
                    }

                    double factor = scaleFactor;
                    double tempFactor = scaleFactor * currentScaleFactor;

                    if (( (tempFactor * width) > (windowWidth * 3)) &&
                          ((tempFactor * height) > (windowHeight * 3))){
                        quit = true;
                    }
                    else if (( (tempFactor * width) < (windowWidth - 120)) &&
                             ((tempFactor * height) < (windowHeight - 120)) ) {
                        quit = true;
                    }
                    else {
                        if(factor * currentScaleFactor > factor)
                            lastZoomIn = factor * currentScaleFactor;
                        else
                            lastZoomOut = factor * currentScaleFactor;
                        factor *= currentScaleFactor;
                        quit = false;
                    }

                    if(quit)
                        return true;

                    this->setMinimumSize(width * factor, height * factor);
                    this->setMaximumSize(width * factor, height * factor);
                    this->setTransform(QTransform().scale(factor, factor));


                    //Let's see if we can still zoom in
                    if((width * factor < windowWidth)
                            && (height * factor < windowHeight)) {
                        blockZoomingIn = true;
                        blockZoomingOut = false;
                    }
                    else if ((width * factor > (windowWidth * 3) )
                             && (height * factor > (windowHeight * 3) )) {
                        blockZoomingIn = false;
                        blockZoomingOut = true;
                    }
                    else {
                        blockZoomingIn = false;
                        blockZoomingOut = false;
                    }
                }
                return true;
            }
            default:
                return QGraphicsWidget::event(event);
        }
    }
    return true;
}

void PaintingArea::setBrushColor(QColor color)
{
    this->color = color;
}

void PaintingArea::setToolType(int type)
{
    this->toolTypeSelected = type;
}

void PaintingArea::setRubberMode(bool mode)
{
    this->rubberMode = mode;
}

void PaintingArea::setLineWidth(int width)
{
    switch (width) {
    case small:
        lineWidthSelected = 3;
        break;
    case medium:
        lineWidthSelected = 6;
        break;
    case large:
        lineWidthSelected = 9;
        break;
    default:
        lineWidthSelected = 6;
    }
}

void PaintingArea::setPanningMode(bool enabled)
{
    this->panningMode = enabled;
}

bool PaintingArea::isImageModified()
{
    return imageModified;
}

void PaintingArea::setIsImageModified(bool isModified)
{
    imageModified = isModified;
    emit this->setSaveNotification(isModified);
}

void PaintingArea::createNewImage()
{
    QSettings settings;

    int value =  settings.value("image/size", 1).toInt();
    if(value > 2 || value < 0)
        value = 1;

    int sWidth, sHeight;

    if (value == 0) {
        sWidth = 700;
        sHeight = 700;
    }
    else if (value == 2) {
        sWidth = 1100;
        sHeight = 1100;
    }
    else {
        sWidth = 900;
        sHeight = 900;
    }


    if(image)
        image->~QImage();
    image = new QImage(sWidth, sHeight, QImage::Format_RGB16);

    QPainter painter(image);
    painter.fillRect(image->rect(), QBrush("white"));
    painter.end();

    this->width = sWidth;
    this->height = sHeight;
    this->resize(sWidth, sHeight);
    this->setGeometry(0, 0, sWidth, sHeight);
    this->setMinimumSize(sWidth, sHeight);
    this->setMaximumSize(sWidth, sHeight);
    standardZoom();
    blockZoomingIn = false;
    blockZoomingOut = false;

    setIsImageModified(false);
}

void PaintingArea::standardZoom()
{
    this->setTransform(QTransform().scale(1, 1));
    this->scaleFactor = 1;
    this->lastZoomOut = 1;
    this->lastZoomOut = 1;
    this->setMinimumSize(width, height);
    this->setMaximumSize(width, height);
}

void PaintingArea::scaleImage(double factor)
{
    int width = image->size().width();
    int height = image->size().height();
    qDebug() << "IM width: " << width << " height: " << height;

    //Get window size:
    int windowWidth = MApplication::activeWindow()->visibleSceneSize().width();
    int windowHeight = MApplication::activeWindow()->visibleSceneSize().height();

    this->setMinimumSize(width * factor, height * factor);
    this->setMaximumSize(width * factor, height * factor);
    this->setTransform(QTransform().scale(factor, factor));

    //Let's see if we can still zoom in
    if((width * factor < windowWidth)
            && (height * factor < windowHeight)) {
        blockZoomingIn = true;
        blockZoomingOut = false;
    }
    else if ((width * factor > (windowWidth * 1.5) )
             && (height * factor > (windowHeight * 1.5) )) {
        blockZoomingIn = false;
        blockZoomingOut = true;
    }
    else {
        blockZoomingIn = false;
        blockZoomingOut = false;
    }

}
