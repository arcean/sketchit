#ifndef PAINTINGAREA_H
#define PAINTINGAREA_H

#include <QWidget>
#include <QGraphicsWidget>
#include <QPainter>
#include <QImage>
#include <QPoint>
#include <QColor>
#include <QGraphicsSceneResizeEvent>
#include <MWidget>

class PaintingArea : public MWidget
{
    Q_OBJECT
public:
    explicit PaintingArea(int width, int height, MWidget *parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setToolType(int type);
    void setRubberMode(bool mode);
    void setLineWidth(int width);
    void setPanningMode(bool enabled);
    bool isImageModified();
    void setIsImageModified(bool isModified);
    void scaleImage(double factor);

protected:
    virtual bool event(QEvent *event);
    /* Commented out in 1.0.2 */
    //void mousePressEvent(QGraphicsSceneMouseEvent *event);
    //void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

signals:
    void setSaveNotification(bool notification);

public slots:
    void setBrushColor(QColor color);
    void openImage (QString newImage);
    void saveImage (QString filename);
    void createNewImage();

private:
    bool isDamaged(QPointF from, QPointF to, int direction);
    void drawLineFromTo(QPointF from, QPointF to);
    void drawFinal(QPointF from, QPointF to);
    void updateLowPoint(QPointF point);
    void updateHighPoint(QPointF point);
    void standardZoom();
    void setActualEreasingAreaRectangle(QPointF from, QPointF to, int direction);

    int width;
    int height;
    QImage *image;
    QImage *backup_image;
    bool ok;
    QColor color;
    enum toolType {finger, pencil, rectangle, elipse, line, rubber};
    enum lineWidth {small, medium, large};
    int toolTypeSelected;
    int lineWidthSelected;
    QPointF touchBegin;
    QPointF lowPoint;
    QPointF highPoint;
    QPointF currPoint;
    QPointF lastPoint;
    int previousDirection;
    int damageCount;
    bool rubberMode;
    bool panningMode;
    bool imageModified;
    MWidget *parent;
    double scaleFactor;
    bool blockZoomingIn;
    bool blockZoomingOut;
    double lastZoomIn;
    double lastZoomOut;
};

#endif // PAINTINGAREA_H
