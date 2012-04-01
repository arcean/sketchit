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
#include <MFeedback>

#define MAX_UNDO 3

class PaintingArea : public MWidget
{
    Q_OBJECT
public:
    explicit PaintingArea(bool feedbackEnabled, MWidget *parent = 0);
    ~PaintingArea();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setToolType(int type);
    void setRubberMode(bool mode);
    void setLineWidth(int width);
    void setPanningMode(bool enabled);
    bool isImageModified();
    void setIsImageModified(bool isModified);
    void scaleImage(double factor);
    void setFeedbackEnabled(bool enabled);

protected:
    virtual bool event(QEvent *event);
    /* Commented out in 1.0.2 */
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

signals:
    void setSaveNotification(bool notification);
    void countRedo(int count_redo);
    void countUndo(int count_undo);
    void pressed();
    void released();
    void moved();

public slots:
    void setBrushColor(QColor color);
    void openImage (QString newImage);
    void saveImage (QString filename);
    void createNewImage();
    void undoPop();
    void undoPush();
    void redoPop();
    void redoPush();
    void resetUndoRedoCounters();

private:
    bool isDamaged(QPointF from, QPointF to, int direction);
    void drawLineFromTo(QPointF from, QPointF to);
    void drawFinal(QPointF from, QPointF to);
    void updateLowPoint(QPointF point);
    void updateHighPoint(QPointF point);
    void standardZoom();
    void setActualEreasingAreaRectangle(QPointF from, QPointF to, int direction);
    void feedbackPressSlot();
    void feedbackReleaseSlot();

    int width;
    int height;
    QPixmap *image;
    QPixmap *backup_image;
    QPixmap *undo_image[MAX_UNDO];
    QPixmap *redo_image[MAX_UNDO];
    int undo_indi;
    int redo_indi;
    int count_undo;
    int count_redo;
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
    bool feedbackEnabled;

    MFeedback *feedbackPress;
};

#endif // PAINTINGAREA_H
